/******************************************************************************\
# H - version                                    #       Maximum Tension       #
################################################################################
#                                                #      -__            __-     #
# Teoman Deniz                                   #  :    :!1!-_    _-!1!:    : #
# maximum-tension.com                            #  ::                      :: #
#                                                #  :!:    : :: : :  :  ::::!: #
# +.....................++.....................+ #   :!:: :!:!1:!:!::1:::!!!:  #
# : C - Maximum Tension :: Create - 2026/11/17 : #   ::!::!!1001010!:!11!!::   #
# :---------------------::---------------------: #   :!1!!11000000000011!!:    #
# : License - GPL-3.0   :: Update - 2026/11/28 : #    ::::!!!1!!1!!!1!!!::     #
# +.....................++.....................+ #       ::::!::!:::!::::      #
\******************************************************************************/

/*
# THE VERSION OF INWAY. IT IS THE RELEASE DATE: YY.YY.MM.DD, SO 2026/11/17 IS
# 20.26.11.17. THE GITHUB RELEASE TAG STARTS WITH THE SAME DATE, ANYTHING MAY
# FOLLOW ("20.26.11.17-MK1151"). THE UPDATER NEVER INSTALLS A RELEASE WITH AN
# OLDER DATE; WHICH ONE IS NEWER IS DECIDED BY THE SHA-256 (SEE UPDATE.H).
# THIS FILE IS ALSO READ BY THE RESOURCE COMPILER, SO IT ONLY HOLDS PLAIN
# DEFINES.
*/

#ifndef VERSION_H
#define VERSION_H

/* ************************* [v] RELEASE NUMBER [v] ************************* */
#define INWAY_VERSION_A 20
#define INWAY_VERSION_B 26
#define INWAY_VERSION_C 11
#define INWAY_VERSION_D 28
#define INWAY_VERSION_TEXT "20.26.11.28"
/* ************************* [^] RELEASE NUMBER [^] ************************* */

#ifdef _WIN64
#define INWAY_BITS_TEXT ""
#else /* !_WIN64 */
#define INWAY_BITS_TEXT " 32-bit" /* SHOWN NEXT TO THE VERSION */
#endif /* _WIN64 */

#endif /* !VERSION_H */
