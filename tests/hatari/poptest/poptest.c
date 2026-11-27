/*
 * poptest.prg - tiny GEM app used by the Hatari test harness.
 *
 * Opens a window at a fixed place. Every left button press inside the
 * work area opens a menu_popup() at the mouse, like Taskbar's configure
 * dialog does (see XaAES issue #213). Everything that happens is logged
 * to the host through the Hatari NatFeats NF_STDERR call, one line per
 * event, prefixed with "POPTEST:", so the host script can assert on it.
 *
 * Keys: 'q' quits. The app also quits by itself after 10 minutes.
 */
#include <gem.h>
#include <osbind.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

/* Work area the host script expects (screen pixels). */
#define WORK_X 160
#define WORK_Y 120
#define WORK_W 320
#define WORK_H 200

/* --- NatFeats ---------------------------------------------------------- */
long nf_id_asm(const char *name) __asm__("poptest_nfid");
long nf_call_asm(long id, ...) __asm__("poptest_nfcall");
__asm__(
	"\t.text\n"
	"\t.even\n"
	"poptest_nfid:\n"
	"\t.word 0x7300\n"
	"\trts\n"
	"poptest_nfcall:\n"
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

	strcpy(buf, "POPTEST: ");
	va_start(ap, fmt);
	n = vsprintf(buf + 9, fmt, ap);
	va_end(ap);
	strcpy(buf + 9 + n, "\n");
	nf_msg = buf;
	Supexec(nf_print_super);
}

/* --- popup tree -------------------------------------------------------- */
#define NITEMS 4
static char *item_text[NITEMS] = { "  Item 1  ", "  Item 2  ", "  Item 3  ", "  Item 4  " };
static OBJECT tree[NITEMS + 1];

static void build_tree(short cw, short ch)
{
	int i;

	memset(tree, 0, sizeof(tree));
	tree[0].ob_next = -1;
	tree[0].ob_head = 1;
	tree[0].ob_tail = NITEMS;
	tree[0].ob_type = G_BOX;
	tree[0].ob_spec.index = 0x00FF1100L;
	tree[0].ob_width = 10 * cw;
	tree[0].ob_height = NITEMS * ch;
	for (i = 1; i <= NITEMS; i++)
	{
		tree[i].ob_next = (i == NITEMS) ? 0 : i + 1;
		tree[i].ob_head = -1;
		tree[i].ob_tail = -1;
		tree[i].ob_type = G_STRING;
		tree[i].ob_flags = (i == NITEMS) ? OF_LASTOB : 0;
		tree[i].ob_spec.free_string = item_text[i - 1];
		tree[i].ob_x = 0;
		tree[i].ob_y = (i - 1) * ch;
		tree[i].ob_width = 10 * cw;
		tree[i].ob_height = ch;
	}
}

/* --- main -------------------------------------------------------------- */
int main(void)
{
	short msg[8], mx, my, bs, ks, key, clicks, ev;
	short wx, wy, ww, wh, wh_handle, vh, cw, ch, bw, bh;
	short work_in[11], work_out[57];
	int popups = 0, buttons = 0, i;
	long start;

	Supexec(nf_init_super);

	if (appl_init() < 0)
		return 1;

	vh = graf_handle(&cw, &ch, &bw, &bh);
	for (i = 0; i < 10; i++)
		work_in[i] = 1;
	work_in[10] = 2;
	v_opnvwk(work_in, &vh, work_out);
	build_tree(cw, ch);

	wind_calc(WC_BORDER, NAME | MOVER, WORK_X, WORK_Y, WORK_W, WORK_H, &wx, &wy, &ww, &wh);
	wh_handle = wind_create(NAME | MOVER, wx, wy, ww, wh);
	wind_set_str(wh_handle, WF_NAME, " Popup test ");
	wind_open(wh_handle, wx, wy, ww, wh);
	graf_mouse(ARROW, NULL);

	logf_("READY work=%d,%d,%d,%d char=%d,%d", WORK_X, WORK_Y, WORK_W, WORK_H, cw, ch);
	start = clock();

	for (;;)
	{
		ev = evnt_multi(MU_BUTTON | MU_KEYBD | MU_MESAG | MU_TIMER,
				2, 1, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				msg, 100L,
				&mx, &my, &bs, &ks, &key, &clicks);

		if (ev & MU_MESAG)
		{
			switch (msg[0])
			{
			case WM_REDRAW:
			{
				GRECT r, w;
				short pxy[4];

				w.g_x = msg[4]; w.g_y = msg[5]; w.g_w = msg[6]; w.g_h = msg[7];
				wind_update(BEG_UPDATE);
				graf_mouse(M_OFF, NULL);
				wind_get(msg[3], WF_FIRSTXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h);
				while (r.g_w && r.g_h)
				{
					if (rc_intersect(&w, &r))
					{
						pxy[0] = r.g_x; pxy[1] = r.g_y;
						pxy[2] = r.g_x + r.g_w - 1; pxy[3] = r.g_y + r.g_h - 1;
						vs_clip(vh, 1, pxy);
						vsf_color(vh, 0);
						vsf_interior(vh, 1);
						v_bar(vh, pxy);
						vs_clip(vh, 0, pxy);
					}
					wind_get(msg[3], WF_NEXTXYWH, &r.g_x, &r.g_y, &r.g_w, &r.g_h);
				}
				graf_mouse(M_ON, NULL);
				wind_update(END_UPDATE);
				break;
			}
			case WM_TOPPED:
				wind_set(msg[3], WF_TOP, 0, 0, 0, 0);
				break;
			case WM_MOVED:
				wind_set(msg[3], WF_CURRXYWH, msg[4], msg[5], msg[6], msg[7]);
				break;
			}
		}

		if (ev & MU_BUTTON)
		{
			buttons++;
			logf_("BUTTON n=%d x=%d y=%d state=%d clicks=%d", buttons, mx, my, bs, clicks);
			if (mx >= WORK_X && mx < WORK_X + WORK_W && my >= WORK_Y && my < WORK_Y + WORK_H)
			{
				MENU in, out;
				short ret;

				memset(&in, 0, sizeof(in));
				memset(&out, 0, sizeof(out));
				in.mn_tree = tree;
				in.mn_menu = 0;
				in.mn_item = 1;
				popups++;
				logf_("POPUP_OPEN n=%d x=%d y=%d", popups, mx, my);
				ret = menu_popup(&in, mx, my, &out);
				graf_mkstate(&mx, &my, &bs, &ks);
				logf_("POPUP_CLOSE n=%d ret=%d item=%d mstate=%d", popups, ret, ret ? out.mn_item : -1, bs);
			}
		}

		if (ev & MU_KEYBD)
		{
			logf_("KEY %04x", (unsigned short)key);
			if ((key & 0xff) == 'q')
				break;
		}

		if (ev & MU_TIMER)
		{
			static short last_bs = -1;

			graf_mkstate(&mx, &my, &bs, &ks);
			if (bs != last_bs)
				logf_("MSTATE %d", bs);
			last_bs = bs;
			if (clock() - start > 600L * CLOCKS_PER_SEC)
				break;
		}
	}

	logf_("DONE popups=%d buttons=%d", popups, buttons);
	wind_close(wh_handle);
	wind_delete(wh_handle);
	v_clsvwk(vh);
	appl_exit();
	return 0;
}
