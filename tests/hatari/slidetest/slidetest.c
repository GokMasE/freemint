/*
 * slidetest.prg - GEM app for the Hatari slider painting test.
 *
 * Opens one window with both sliders, arrows, sizer and fuller. On
 * key 'n' it runs the next scripted step: it changes the sliders with
 * wind_set(), in some steps right after a wind_set(WF_CURRXYWH) resize,
 * which leaves a frame redraw queued (the case where XaAES skips the
 * direct slider paint, see display_slider() in xa_wind.c). On 'r' it
 * forces a full repaint of the window (form_dial FMD_FINISH over it),
 * so the host can compare "as painted" against "as repainted from
 * scratch" ('g' first scribbles over the window, to prove that 'r'
 * really repaints the frame). WM_SIZED (sizer drag) is answered with WF_CURRXYWH followed
 * by slider updates, like a typical app during a live resize.
 *
 * Everything is logged to the host via NatFeats NF_STDERR, one line per
 * event, prefixed "SLIDETEST:". Keys: 'q' quits.
 */
#include <gem.h>
#include <osbind.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define KIND (NAME | MOVER | CLOSER | FULLER | SIZER | \
	      VSLIDE | UPARROW | DNARROW | HSLIDE | LFARROW | RTARROW)
#define WIN_X 40
#define WIN_Y 40
#define WIN_W 360
#define WIN_H 260

/* --- NatFeats ---------------------------------------------------------- */
long nf_id_asm(const char *name) __asm__("slidetest_nfid");
long nf_call_asm(long id, ...) __asm__("slidetest_nfcall");
__asm__(
	"\t.text\n"
	"\t.even\n"
	"slidetest_nfid:\n"
	"\t.word 0x7300\n"
	"\trts\n"
	"slidetest_nfcall:\n"
	"\t.word 0x7301\n"
	"\trts\n");

static long nf_stderr_id;
static const char *nf_msg;

static long nf_init_super(void)
{
	nf_stderr_id = nf_id_asm("NF_STDERR");
	return 0;
}

static long nf_print_super(void)
{
	if (nf_stderr_id)
		nf_call_asm(nf_stderr_id, nf_msg);
	return 0;
}

static void logf_(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
static void logf_(const char *fmt, ...)
{
	char buf[200];
	va_list ap;
	int n;

	strcpy(buf, "SLIDETEST: ");
	va_start(ap, fmt);
	n = vsprintf(buf + 11, fmt, ap);
	va_end(ap);
	strcpy(buf + 11 + n, "\n");
	nf_msg = buf;
	Supexec(nf_print_super);
}

static short wh, vh;

static void sliders(short vpos, short vsize, short hpos, short hsize)
{
	if (vpos >= 0)  wind_set(wh, WF_VSLIDE, vpos, 0, 0, 0);
	if (vsize >= 0) wind_set(wh, WF_VSLSIZE, vsize, 0, 0, 0);
	if (hpos >= 0)  wind_set(wh, WF_HSLIDE, hpos, 0, 0, 0);
	if (hsize >= 0) wind_set(wh, WF_HSLSIZE, hsize, 0, 0, 0);
}

static void log_rect(const char *what, int n)
{
	short x, y, w, h, wx, wy, ww, whh;

	wind_get(wh, WF_CURRXYWH, &x, &y, &w, &h);
	wind_get(wh, WF_WORKXYWH, &wx, &wy, &ww, &whh);
	logf_("%s %d rect=%d,%d,%d,%d work=%d,%d,%d,%d", what, n, x, y, w, h, wx, wy, ww, whh);
}

/* Slider values that follow the window size, as an app would set them. */
static void sliders_for_size(short w, short h)
{
	sliders((w * 3) % 1001, (h * 4) % 1001, (h * 5) % 1001, (w * 2) % 1001);
}

static void step(int n)
{
	switch (n)
	{
	case 1:	/* plain slider moves, no redraw queued */
		sliders(1000, -1, -1, -1);
		break;
	case 2:
		sliders(-1, 600, 700, 200);
		break;
	case 3:	/* grow, then set sliders while the frame redraw is queued */
		wind_set(wh, WF_CURRXYWH, WIN_X, WIN_Y, 420, 320);
		sliders(400, 250, 300, 600);
		break;
	case 4:	/* shrink, then set sliders while the frame redraw is queued */
		wind_set(wh, WF_CURRXYWH, WIN_X, WIN_Y, 300, 220);
		sliders(800, -1, 1000, -1);
		break;
	case 5:
		wind_set(wh, WF_CURRXYWH, WIN_X, WIN_Y, 280, 200);
		sliders(-1, 900, -1, 900);
		break;
	case 6:	/* move only, then sliders */
		wind_set(wh, WF_CURRXYWH, WIN_X + 30, WIN_Y + 20, 280, 200);
		sliders(0, 100, 0, 100);
		break;
	default:
		logf_("NOSTEP %d", n);
		return;
	}
	log_rect("STEP", n);
}

static void redraw(short handle, GRECT *area)
{
	GRECT r;
	short pxy[4];

	wind_update(BEG_UPDATE);
	graf_mouse(M_OFF, NULL);
	wind_get(handle, WF_FIRSTXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h);
	while (r.g_w && r.g_h)
	{
		if (rc_intersect(area, &r))
		{
			pxy[0] = r.g_x; pxy[1] = r.g_y;
			pxy[2] = r.g_x + r.g_w - 1; pxy[3] = r.g_y + r.g_h - 1;
			vs_clip(vh, 1, pxy);
			vsf_color(vh, 0);
			vsf_interior(vh, 1);
			v_bar(vh, pxy);
			vs_clip(vh, 0, pxy);
		}
		wind_get(handle, WF_NEXTXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h);
	}
	graf_mouse(M_ON, NULL);
	wind_update(END_UPDATE);
}

int main(void)
{
	short msg[8], mx, my, bs, ks, key, clicks, ev;
	short cw, ch, bw, bh, x, y, w, h;
	short work_in[11], work_out[57];
	int n = 0, refs = 0, sized = 0, i;
	long start;

	Supexec(nf_init_super);
	if (appl_init() < 0)
		return 1;

	vh = graf_handle(&cw, &ch, &bw, &bh);
	for (i = 0; i < 10; i++)
		work_in[i] = 1;
	work_in[10] = 2;
	v_opnvwk(work_in, &vh, work_out);

	wh = wind_create(KIND, WIN_X, WIN_Y, WIN_W, WIN_H);
	wind_set_str(wh, WF_NAME, " Slider test ");
	sliders(0, 300, 0, 500);
	wind_open(wh, WIN_X, WIN_Y, WIN_W, WIN_H);
	graf_mouse(ARROW, NULL);
	log_rect("READY", 0);
	start = clock();

	for (;;)
	{
		ev = evnt_multi(MU_KEYBD | MU_MESAG | MU_TIMER,
				0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				msg, 100L,
				&mx, &my, &bs, &ks, &key, &clicks);

		if (ev & MU_MESAG)
		{
			switch (msg[0])
			{
			case WM_REDRAW:
			{
				GRECT a;
				a.g_x = msg[4]; a.g_y = msg[5]; a.g_w = msg[6]; a.g_h = msg[7];
				redraw(msg[3], &a);
				break;
			}
			case WM_TOPPED:
				wind_set(msg[3], WF_TOP, 0, 0, 0, 0);
				break;
			case WM_MOVED:
				wind_set(msg[3], WF_CURRXYWH, msg[4], msg[5], msg[6], msg[7]);
				break;
			case WM_SIZED:
				sized++;
				wind_set(msg[3], WF_CURRXYWH, msg[4], msg[5], msg[6], msg[7]);
				sliders_for_size(msg[6], msg[7]);
				logf_("SIZED %d %d,%d,%d,%d", sized, msg[4], msg[5], msg[6], msg[7]);
				break;
			}
		}

		if (ev & MU_KEYBD)
		{
			char c = key & 0xff;

			logf_("KEY %04x", (unsigned short)key);
			if (c == 'q')
				break;
			if (c == 'n')
				step(++n);
			if (c == 'p')	/* report the current rect (after a drag) */
				log_rect("RECT", sized);
			if (c == 'g')	/* scribble over the whole window, frame too */
			{
				short pxy[4];
				wind_get(wh, WF_CURRXYWH, &x, &y, &w, &h);
				pxy[0] = x; pxy[1] = y; pxy[2] = x + w - 1; pxy[3] = y + h - 1;
				wind_update(BEG_UPDATE);
				graf_mouse(M_OFF, NULL);
				vs_clip(vh, 0, pxy);
				vsf_color(vh, 1);
				vsf_interior(vh, 2);
				vsf_style(vh, 4);
				v_bar(vh, pxy);
				graf_mouse(M_ON, NULL);
				wind_update(END_UPDATE);
				log_rect("GARBAGE", 0);
			}
			if (c == 'r')
			{
				wind_get(wh, WF_CURRXYWH, &x, &y, &w, &h);
				form_dial(FMD_FINISH, 0, 0, 0, 0, x, y, w, h);
				refs++;
				log_rect("REF", refs);
			}
		}

		if ((ev & MU_TIMER) && clock() - start > 900L * CLOCKS_PER_SEC)
			break;
	}

	logf_("DONE steps=%d refs=%d sized=%d", n, refs, sized);
	wind_close(wh);
	wind_delete(wh);
	v_clsvwk(vh);
	appl_exit();
	return 0;
}
