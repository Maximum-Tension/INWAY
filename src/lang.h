/******************************************************************************\
# H - lang                                       #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GPL-3.0   :: Update - 2026/11/17 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/*
# EVERY SENTENCE INWAY SHOWS, IN ENGLISH AND TURKISH. THE LANGUAGE COMES FROM
# THE WINDOWS DISPLAY LANGUAGE; ANYTHING THAT IS NOT TURKISH GETS ENGLISH.
# "-LANG EN" OR "-LANG TR" ON THE COMMAND LINE OVERRIDES IT.
# TO ADD A LANGUAGE: ADD A TABLE IN LANG.C IN THE SAME ORDER AS E_STR, AND PICK
# IT IN LANG_INIT(). STRINGS ARE UTF-8. "%S" IS USUALLY THE SHORT ADDRESS OF A
# PC, LIKE 2.253.
*/

#ifndef LANG_H
#define LANG_H

enum	E_STR
{
	/* MAIN SCREEN */
	STR_SHARE_TITLE,
	STR_ADDRESS,
	STR_CODE,
	STR_NO_NETWORK,
	STR_SHARE_HINT,
	STR_SHARE_OFF,
	STR_SHARE_OFF_HINT,
	STR_SVC_HINT,
	STR_SVC_CODE_UNKNOWN,
	STR_STATUS_CONTROLLING,
	STR_CONNECT_TITLE,
	STR_CONNECT_HINT,
	STR_EXAMPLE,
	STR_CLOSING_STOPS,
	/* QUESTIONS */
	STR_ASK_ADDRESS,
	STR_BAD_ADDRESS,
	STR_SELF_ADDRESS,
	STR_CODE_HINT,
	STR_ASK_CODE,
	STR_CODE_6,
	STR_CONNECTING,
	STR_WAITING,
	STR_WRONG_CODE_RETRY,
	STR_NEED_CODE_RETRY,
	STR_CONNECTED,
	STR_CONNECTED_HINT,
	/* RESULTS */
	STR_R_ENDED,
	STR_R_DECLINED_T,
	STR_R_DECLINED,
	STR_R_NO_ANSWER_T,
	STR_R_NO_ANSWER,
	STR_R_BUSY_T,
	STR_R_BUSY,
	STR_R_NO_PC_T,
	STR_R_NO_PC,
	STR_R_NO_PC_1,
	STR_R_NO_PC_2,
	STR_R_NO_PC_3,
	STR_R_DROPPED,
	STR_R_ERROR,
	STR_R_WRONG_T,
	STR_R_WRONG,
	STR_R_NEED_T,
	/* SHARING SIDE */
	STR_N_ASKING,
	STR_N_REFUSED,
	STR_N_GAVE_UP,
	STR_N_WRONG,
	STR_N_CONTROLLING,
	STR_N_STOPPED,
	/* POPUP WINDOWS */
	STR_H_TITLE,
	STR_H_TEXT,
	STR_H_ACCEPT,
	STR_H_CANCEL,
	STR_H_COUNTDOWN,
	STR_H_DISCONNECT,
	/* REMOTE SCREEN WINDOW */
	STR_V_TITLE,
	STR_V_MENU_CAD,
	STR_V_REMOTE_CLOSED,
	/* INSTALL */
	STR_I_NEEDS_CODE,
	STR_I_PERMISSION,
	STR_I_NO_PERMISSION,
	STR_I_TITLE_INSTALL,
	STR_I_TITLE_REMOVE,
	STR_I_TITLE_OLD,
	STR_I_NO_SCM,
	STR_I_STOPPING_OLD,
	STR_I_COPYING,
	STR_I_COPY_FAIL,
	STR_I_SETUP,
	STR_I_CREATE_FAIL,
	STR_I_FIREWALL,
	STR_I_NOT_STARTED,
	STR_I_STOPPING,
	STR_I_NOT_INSTALLED,
	STR_I_OLD_NOT_FOUND,
	STR_I_DONE_T,
	STR_I_DONE,
	STR_I_REMOVE_HINT,
	STR_I_REMOVED,
	STR_I_OLD_REMOVED,
	STR_I_TAKEOVER,
	STR_I_PERMISSION_INSTALL,
	STR_I_USER_SETUP,
	STR_I_USER_FAIL,
	STR_I_USER_DONE,
	STR_I_USER_UPGRADE,
	STR_I_USER_REMOVED,
	/* MISC */
	STR_PRESS_ENTER_CLOSE,
	STR_PRESS_ENTER,
	STR_E_P,
	STR_E_C,
	STR_E_UNKNOWN,
	STR_E_NET,
	STR_HELP,
	STR_U_FOUND,
	STR_U_FAILED,
	STR_U_DONE,
	/* CLIPBOARD */
	STR_C_SENT,
	STR_C_GOT,
	STR_C_TOO_BIG,
	STR_C_CANT_READ,
	/* ADMIN RIGHTS FOR ONE SESSION */
	STR_N_ADMIN_ASK,
	STR_N_ADMIN_NO,
	STR_SVC_USER_HINT,
	/* A SERVER UPDATES BEFORE A SESSION */
	STR_UPDATING_WAIT,
	STR_R_UPDATING_T,
	STR_R_UPDATING,
	STR__COUNT
};

extern void			LANG_INIT(const char *FORCE);
extern const char	*LANG(enum E_STR ID);

#endif /* !LANG_H */
