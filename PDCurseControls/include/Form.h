#pragma once
#include "Controllable.h"
#include "FormField.h"

/***************************
*  miscellaneous #defines  *
amm: ripped straight from ncurses form.h file
***************************/

/* form options */
#define O_NL_OVERLOAD		(0x0001U)
#define O_BS_OVERLOAD		(0x0002U)

/* form driver commands */
#define REQ_NEXT_PAGE	 (KEY_MAX + 1)	/* move to next page		*/
#define REQ_PREV_PAGE	 (KEY_MAX + 2)	/* move to previous page	*/
#define REQ_FIRST_PAGE	 (KEY_MAX + 3)	/* move to first page		*/
#define REQ_LAST_PAGE	 (KEY_MAX + 4)	/* move to last page		*/

#define REQ_NEXT_FIELD	 (KEY_MAX + 5)	/* move to next field		*/
#define REQ_PREV_FIELD	 (KEY_MAX + 6)	/* move to previous field	*/
#define REQ_FIRST_FIELD	 (KEY_MAX + 7)	/* move to first field		*/
#define REQ_LAST_FIELD	 (KEY_MAX + 8)	/* move to last field		*/
#define REQ_SNEXT_FIELD	 (KEY_MAX + 9)	/* move to sorted next field	*/
#define REQ_SPREV_FIELD	 (KEY_MAX + 10)	/* move to sorted prev field	*/
#define REQ_SFIRST_FIELD (KEY_MAX + 11)	/* move to sorted first field	*/
#define REQ_SLAST_FIELD	 (KEY_MAX + 12)	/* move to sorted last field	*/
#define REQ_LEFT_FIELD	 (KEY_MAX + 13)	/* move to left to field	*/
#define REQ_RIGHT_FIELD	 (KEY_MAX + 14)	/* move to right to field	*/
#define REQ_UP_FIELD	 (KEY_MAX + 15)	/* move to up to field		*/
#define REQ_DOWN_FIELD	 (KEY_MAX + 16)	/* move to down to field	*/

#define REQ_NEXT_CHAR	 (KEY_MAX + 17)	/* move to next char in field	*/
#define REQ_PREV_CHAR	 (KEY_MAX + 18)	/* move to prev char in field	*/
#define REQ_NEXT_LINE	 (KEY_MAX + 19)	/* move to next line in field	*/
#define REQ_PREV_LINE	 (KEY_MAX + 20)	/* move to prev line in field	*/
#define REQ_NEXT_WORD	 (KEY_MAX + 21)	/* move to next word in field	*/
#define REQ_PREV_WORD	 (KEY_MAX + 22)	/* move to prev word in field	*/
#define REQ_BEG_FIELD	 (KEY_MAX + 23)	/* move to first char in field	*/
#define REQ_END_FIELD	 (KEY_MAX + 24)	/* move after last char in fld	*/
#define REQ_BEG_LINE	 (KEY_MAX + 25)	/* move to beginning of line	*/
#define REQ_END_LINE	 (KEY_MAX + 26)	/* move after last char in line	*/
#define REQ_LEFT_CHAR	 (KEY_MAX + 27)	/* move left in field		*/
#define REQ_RIGHT_CHAR	 (KEY_MAX + 28)	/* move right in field		*/
#define REQ_UP_CHAR	 (KEY_MAX + 29)	/* move up in field		*/
#define REQ_DOWN_CHAR	 (KEY_MAX + 30)	/* move down in field		*/

#define REQ_NEW_LINE	 (KEY_MAX + 31)	/* insert/overlay new line	*/
#define REQ_INS_CHAR	 (KEY_MAX + 32)	/* insert blank char at cursor	*/
#define REQ_INS_LINE	 (KEY_MAX + 33)	/* insert blank line at cursor	*/
#define REQ_DEL_CHAR	 (KEY_MAX + 34)	/* delete char at cursor	*/
#define REQ_DEL_PREV	 (KEY_MAX + 35)	/* delete char before cursor	*/
#define REQ_DEL_LINE	 (KEY_MAX + 36)	/* delete line at cursor	*/
#define REQ_DEL_WORD	 (KEY_MAX + 37)	/* delete word at cursor	*/
#define REQ_CLR_EOL	 (KEY_MAX + 38)	/* clear to end of line		*/
#define REQ_CLR_EOF	 (KEY_MAX + 39)	/* clear to end of field	*/
#define REQ_CLR_FIELD	 (KEY_MAX + 40)	/* clear entire field		*/
#define REQ_OVL_MODE	 (KEY_MAX + 41)	/* begin overlay mode		*/
#define REQ_INS_MODE	 (KEY_MAX + 42)	/* begin insert mode		*/
#define REQ_SCR_FLINE	 (KEY_MAX + 43)	/* scroll field forward a line	*/
#define REQ_SCR_BLINE	 (KEY_MAX + 44)	/* scroll field backward a line	*/
#define REQ_SCR_FPAGE	 (KEY_MAX + 45)	/* scroll field forward a page	*/
#define REQ_SCR_BPAGE	 (KEY_MAX + 46)	/* scroll field backward a page	*/
#define REQ_SCR_FHPAGE	 (KEY_MAX + 47) /* scroll field forward	 half page */
#define REQ_SCR_BHPAGE	 (KEY_MAX + 48) /* scroll field backward half page */
#define REQ_SCR_FCHAR	 (KEY_MAX + 49) /* horizontal scroll char	*/
#define REQ_SCR_BCHAR	 (KEY_MAX + 50) /* horizontal scroll char	*/
#define REQ_SCR_HFLINE	 (KEY_MAX + 51) /* horizontal scroll line	*/
#define REQ_SCR_HBLINE	 (KEY_MAX + 52) /* horizontal scroll line	*/
#define REQ_SCR_HFHALF	 (KEY_MAX + 53) /* horizontal scroll half line	*/
#define REQ_SCR_HBHALF	 (KEY_MAX + 54) /* horizontal scroll half line	*/

#define REQ_VALIDATION	 (KEY_MAX + 55)	/* validate field		*/
#define REQ_NEXT_CHOICE	 (KEY_MAX + 56)	/* display next field choice	*/
#define REQ_PREV_CHOICE	 (KEY_MAX + 57)	/* display prev field choice	*/

#define MIN_FORM_COMMAND (KEY_MAX + 1)	/* used by form_driver		*/
#define MAX_FORM_COMMAND (KEY_MAX + 57)	/* used by form_driver		*/



class Form : public Controllable
{
private:
	unsigned short capacity;
	FormField** fields;

public:
	Form(WINDOW* win, int capacity);
	void draw();
};
