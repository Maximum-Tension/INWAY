/******************************************************************************\
# C - lang                                       #       Maximum Tension       #
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
# THE ENGLISH AND TURKISH STRING TABLES. THE TABLES USE [STR_NAME] = "..." SO
# THE ORDER CAN NEVER GET MIXED UP. A MISSING TURKISH STRING FALLS BACK TO
# ENGLISH. COLOUR MARKS LIKE {Y} BELONG TO THE CALLER, NOT TO THE STRINGS.
*/

/* **************************** [v] INCLUDES [v] **************************** */
#include "inway.h"
/******* """""""""
 * #define _WIN32_WINNT
 * #define WIN32_LEAN_AND_MEAN
 */
#include <windows.h>
/******* """""""""""
 * #define LANG_TURKISH
 * #define PRIMARYLANGID(lgid)
 * typedef	LANGID;
 * LANGID	GetUserDefaultUILanguage(void);
 */
#include "lang.h"
/******* """"""""
 * enum E_STR;
 */
/* **************************** [^] INCLUDES [^] **************************** */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                         ENGLISH                          |  \\//  */
/*   \/   #==========================================================#   \/   */
static const char	*const G__EN__[STR__COUNT] =
{
	[STR_SHARE_TITLE] = "LET SOMEONE CONTROL THIS PC",
	[STR_ADDRESS] = "Address",
	[STR_CODE] = "Code",
	[STR_NO_NETWORK] = "no network found",
	[STR_SHARE_HINT] =
		"Give them the code, or just click Accept when the window pops up.",
	[STR_SHARE_OFF] = "Sharing is OFF.",
	[STR_SHARE_OFF_HINT] =
		"Is INWAY already open in another window? Close it, then open "
		"this again.",
	[STR_SVC_HINT] = "Always on, also on the lock screen.",
	[STR_SVC_CODE_UNKNOWN] = "(set at install)",
	[STR_STATUS_CONTROLLING] = "%s is controlling this PC",
	[STR_CONNECT_TITLE] = "CONTROL ANOTHER PC",
	[STR_CONNECT_HINT] =
		"Type the Address or the Name shown on the other PC, then press "
		"Enter.",
	[STR_EXAMPLE] = "Example: 2.253 or OFIS-PC",
	[STR_CLOSING_STOPS] = "Closing this window stops everything.",

	[STR_ASK_ADDRESS] = "Address of the other PC:",
	[STR_BAD_ADDRESS] =
		"That doesn't look right. Type it like this: 2.253 or OFIS-PC",
	[STR_SELF_ADDRESS] =
		"That is THIS PC's address. Type the one shown on the OTHER PC.",
	[STR_CODE_HINT] =
		"If you have the code, type it. If not, just press Enter and %s "
		"will be asked to click Accept.",
	[STR_ASK_CODE] = "Code:",
	[STR_CODE_6] = "The code has 6 numbers, like 123 456. Try again.",
	[STR_CONNECTING] = "Connecting to %s ...",
	[STR_WAITING] = "Waiting for someone at %s to click Accept ...",
	[STR_WRONG_CODE_RETRY] =
		"That code is not right. Look at the code on the other PC's screen.",
	[STR_NEED_CODE_RETRY] =
		"%s is a server: it only lets you in with its code.",
	[STR_CONNECTED] = "Connected! The other screen is open in a new window.",
	[STR_CONNECTED_HINT] =
		"Close that window to stop. For Ctrl+Alt+Del, use the menu at its "
		"top.",

	[STR_R_ENDED] = "You were disconnected from %s.",
	[STR_R_DECLINED_T] = "NOT ALLOWED",
	[STR_R_DECLINED] = "Someone at %s clicked Cancel.",
	[STR_R_NO_ANSWER_T] = "NO ANSWER",
	[STR_R_NO_ANSWER] =
		"Nobody clicked Accept on %s. Ask them to watch their screen, then "
		"try again.",
	[STR_R_BUSY_T] = "BUSY",
	[STR_R_BUSY] =
		"%s is already being controlled by someone else. Try again later.",
	[STR_R_NO_PC_T] = "COULD NOT CONNECT",
	[STR_R_NO_PC] = "No PC answered at %s. Check that:",
	[STR_R_NO_PC_1] = " - INWAY is open on that PC",
	[STR_R_NO_PC_2] = " - you typed the right address or name",
	[STR_R_NO_PC_3] =
		" - that PC clicked \"Allow access\" when Windows asked about it",
	[STR_R_DROPPED] =
		"The other PC hung up. Make sure both PCs have the newest INWAY.",
	[STR_R_ERROR] = "Something went wrong. Try again.",
	[STR_R_WRONG_T] = "WRONG CODE",
	[STR_R_WRONG] = "That code is not right. Look at the code on %s's screen.",
	[STR_R_NEED_T] = "CODE NEEDED",

	[STR_N_ASKING] =
		"%s is asking to control this PC. Click Accept or Cancel.",
	[STR_N_REFUSED] = "You did not let %s in.",
	[STR_N_GAVE_UP] = "%s stopped asking.",
	[STR_N_WRONG] = "%s typed a wrong code.",
	[STR_N_CONTROLLING] = "%s is now controlling this PC.",
	[STR_N_STOPPED] = "%s stopped controlling this PC.",

	[STR_H_TITLE] = "%s wants to control this PC",
	[STR_H_TEXT] =
		"They will see your screen and use your mouse and keyboard.",
	[STR_H_ACCEPT] = "Accept",
	[STR_H_CANCEL] = "Cancel",
	[STR_H_COUNTDOWN] = "This window closes by itself in %d seconds.",
	[STR_H_DISCONNECT] = "Disconnect",

	[STR_V_TITLE] = "Controlling %s   -   close this window to stop",
	[STR_V_MENU_CAD] = "Send Ctrl+Alt+Del   (or press Ctrl+Alt+End)",
	[STR_V_REMOTE_CLOSED] = "The other PC ended the connection.",

	[STR_I_NEEDS_CODE] =
		"-install needs a code, like:  INWAY.exe -install -p 938572",
	[STR_I_PERMISSION] = "Windows will ask for permission now. Click \"Yes\".",
	[STR_I_NO_PERMISSION] =
		"Permission was not given, so nothing was changed.",
	[STR_I_TITLE_INSTALL] = "INSTALL BACKGROUND SHARING",
	[STR_I_TITLE_REMOVE] = "REMOVE BACKGROUND SHARING",
	[STR_I_TITLE_OLD] = "REMOVE THE OLD CONNECT",
	[STR_I_NO_SCM] = "Could not open the Windows service list.",
	[STR_I_STOPPING_OLD] = "Stopping the old background sharing ...",
	[STR_I_COPYING] = "Copying INWAY to %s ...",
	[STR_I_COPY_FAIL] = "Could not copy INWAY there.",
	[STR_I_SETUP] = "Setting up the background sharing ...",
	[STR_I_CREATE_FAIL] = "Could not create the service (error %lu).",
	[STR_I_FIREWALL] =
		"Allowing it through the Windows firewall (this network only) ...",
	[STR_I_NOT_STARTED] = "Installed, but it did not start. Restart the PC.",
	[STR_I_STOPPING] = "Stopping the background sharing ...",
	[STR_I_NOT_INSTALLED] = "The background sharing was not installed.",
	[STR_I_OLD_NOT_FOUND] = "The old CONNECT was not installed on this PC.",
	[STR_I_DONE_T] = "DONE",
	[STR_I_DONE] =
		"This PC is now shared all the time, also on the lock screen and "
		"after a restart.",
	[STR_I_REMOVE_HINT] = "To remove it:  INWAY.exe -uninstall",
	[STR_I_REMOVED] = "Background sharing is removed.",
	[STR_I_OLD_REMOVED] = "The old CONNECT is removed.",
	[STR_I_TAKEOVER] =
		"Is INWAY also open in a normal window on this PC? Close it: the "
		"background sharing takes over in a few seconds.",
	[STR_I_PERMISSION_INSTALL] =
		"Windows asks for admin rights now. Yes: the full server, also on "
		"the lock screen. No: a normal server that runs while you are "
		"signed in.",
	[STR_I_USER_SETUP] = "Setting it up to start when you sign in ...",
	[STR_I_USER_FAIL] = "Could not set it up to start when you sign in.",
	[STR_I_USER_DONE] =
		"This PC is now shared while you are signed in, with normal rights: "
		"admin windows and the lock screen can not be used.",
	[STR_I_USER_UPGRADE] =
		"For the full server, run the same -install again and click Yes.",
	[STR_I_USER_REMOVED] = "The normal server is removed.",

	[STR_PRESS_ENTER_CLOSE] = "Press Enter to close.",
	[STR_PRESS_ENTER] = "Press Enter to continue.",
	[STR_E_P] = "-p needs a 6-number code, like:  -p 938572",
	[STR_E_C] = "-c needs an address or a name, like:  -c 2.253",
	[STR_E_UNKNOWN] = "Unknown option: %s",
	[STR_E_NET] = "Network start-up failed.",
	[STR_HELP] =
		"  INWAY.exe                     share this PC and connect to others\n"
		"  INWAY.exe -p 938572           use this code for this PC\n"
		"  INWAY.exe -c 2.253 948248     connect to 2.253 once, with a code\n"
		"  INWAY.exe -c 2.253            connect to 2.253 once, they Accept\n"
		"  INWAY.exe -c OFIS-PC 948248   a PC name works too\n"
		"  INWAY.exe -install -p 938572  share all the time, also on the\n"
		"                                lock screen (for servers)\n"
		"  INWAY.exe -install            same, with a random code (or the\n"
		"                                one the server already has)\n"
		"      ... -install -p 938572 -noupdate   same, but no auto update\n"
		"  INWAY.exe -uninstall          remove that again\n"
		"  INWAY.exe -removeold          remove the old CONNECT tool\n"
		"  INWAY.exe -lang tr            language: en or tr\n"
		"  INWAY.exe -noupdate           do not look for a new version\n"
		"\n"
		"  Use INWAY only on PCs you own or are allowed to control.\n"
		"  See DISCLAIMER.md.\n",
	[STR_U_FOUND] = "A new INWAY is out (%s). Updating ...",
	[STR_U_FAILED] = "The update did not work. Starting this version.",
	[STR_U_DONE] = "INWAY is updated to %s.",
	[STR_C_SENT] = "The files you copied are ready to paste on %s.",
	[STR_C_GOT] = "Files copied on %s are ready to paste here.",
	[STR_C_TOO_BIG] = "Those files are over 4 GB, too big to copy to %s.",
	[STR_C_CANT_READ] =
		"Files were copied, but INWAY could not read them to send to %s.",
	[STR_N_ADMIN_ASK] =
		"Windows asks now if %s may use admin rights. Yes: full control. "
		"No: normal rights.",
	[STR_N_ADMIN_NO] =
		"%s has normal rights: admin windows can not be used.",
	[STR_SVC_USER_HINT] = "Always on while you are signed in.",
	[STR_UPDATING_WAIT] =
		"%s is updating INWAY first. Connecting again in a moment ...",
	[STR_R_UPDATING_T] = "UPDATING",
	[STR_R_UPDATING] =
		"%s is still updating INWAY. Try again in a minute.",
	[STR_NAME] = "Name",
	[STR_LOOKING] = "Looking for %s on this network ...",
	[STR_NO_SUCH_NAME] =
		"No PC called %s was found on this network. Try its Address."
};
/*   /\   #==========================================================#   /\   */
/*  //\\  |                         ENGLISH                          |  //\\  */
/* '""""' #==========================================================# '""""' */

/* .,,,,. #==========================================================# .,,,,. */
/*  \\//  |                         TURKISH                          |  \\//  */
/*   \/   #==========================================================#   \/   */
static const char	*const G__TR__[STR__COUNT] =
{
	[STR_SHARE_TITLE] = "BU BİLGİSAYARI BAŞKASI KONTROL ETSİN",
	[STR_ADDRESS] = "Adres",
	[STR_CODE] = "Kod",
	[STR_NO_NETWORK] = "ağ bulunamadı",
	[STR_SHARE_HINT] =
		"Karşı tarafa kodu verin ya da açılan pencerede Kabul Et'e tıklayın.",
	[STR_SHARE_OFF] = "Paylaşım KAPALI.",
	[STR_SHARE_OFF_HINT] =
		"INWAY başka bir pencerede açık mı? Onu kapatıp bunu yeniden açın.",
	[STR_SVC_HINT] = "Her zaman açık, kilit ekranında da.",
	[STR_SVC_CODE_UNKNOWN] = "(kurulumda belirlendi)",
	[STR_STATUS_CONTROLLING] = "%s bu bilgisayarı kontrol ediyor",
	[STR_CONNECT_TITLE] = "BAŞKA BİR BİLGİSAYARA BAĞLAN",
	[STR_CONNECT_HINT] =
		"Diğer bilgisayarın ekranında yazan Adresi ya da Adı yazıp "
		"Enter'a basın.",
	[STR_EXAMPLE] = "Örnek: 2.253 ya da OFIS-PC",
	[STR_CLOSING_STOPS] = "Bu pencereyi kapatmak her şeyi durdurur.",

	[STR_ASK_ADDRESS] = "Diğer bilgisayarın adresi:",
	[STR_BAD_ADDRESS] =
		"Bu doğru görünmüyor. Şöyle yazın: 2.253 ya da OFIS-PC",
	[STR_SELF_ADDRESS] =
		"Bu, BU bilgisayarın adresi. DİĞER bilgisayarda yazanı girin.",
	[STR_CODE_HINT] =
		"Kodu biliyorsanız yazın. Bilmiyorsanız sadece Enter'a basın; "
		"%s üzerindeki kişiden Kabul Et'e tıklaması istenir.",
	[STR_ASK_CODE] = "Kod:",
	[STR_CODE_6] = "Kod 6 rakamdan oluşur, örneğin 123 456. Tekrar deneyin.",
	[STR_CONNECTING] = "%s adresine bağlanılıyor ...",
	[STR_WAITING] = "%s üzerinde birinin Kabul Et'e tıklaması bekleniyor ...",
	[STR_WRONG_CODE_RETRY] =
		"Bu kod doğru değil. Diğer bilgisayarın ekranındaki koda bakın.",
	[STR_NEED_CODE_RETRY] =
		"%s bir sunucu: yalnızca kendi koduyla içeri alır.",
	[STR_CONNECTED] = "Bağlandı! Diğer ekran yeni bir pencerede açıldı.",
	[STR_CONNECTED_HINT] =
		"Durdurmak için o pencereyi kapatın. Ctrl+Alt+Del için üstteki "
		"menüyü kullanın.",

	[STR_R_ENDED] = "%s ile bağlantı kesildi.",
	[STR_R_DECLINED_T] = "İZİN VERİLMEDİ",
	[STR_R_DECLINED] = "%s üzerindeki kişi İptal'e tıkladı.",
	[STR_R_NO_ANSWER_T] = "YANIT YOK",
	[STR_R_NO_ANSWER] =
		"%s üzerinde kimse Kabul Et'e tıklamadı. Ekranlarına bakmalarını "
		"isteyip tekrar deneyin.",
	[STR_R_BUSY_T] = "MEŞGUL",
	[STR_R_BUSY] =
		"%s şu an başka biri tarafından kontrol ediliyor. Daha sonra "
		"tekrar deneyin.",
	[STR_R_NO_PC_T] = "BAĞLANILAMADI",
	[STR_R_NO_PC] = "%s adresinde yanıt veren bilgisayar yok. Şunlara bakın:",
	[STR_R_NO_PC_1] = " - INWAY o bilgisayarda açık mı",
	[STR_R_NO_PC_2] = " - adresi ya da adı doğru yazdınız mı",
	[STR_R_NO_PC_3] =
		" - Windows sorduğunda o bilgisayarda \"Erişime izin ver\" "
		"tıklandı mı",
	[STR_R_DROPPED] =
		"Diğer bilgisayar bağlantıyı kapattı. İki bilgisayarda da en yeni "
		"INWAY olduğundan emin olun.",
	[STR_R_ERROR] = "Bir şeyler ters gitti. Tekrar deneyin.",
	[STR_R_WRONG_T] = "YANLIŞ KOD",
	[STR_R_WRONG] = "Bu kod doğru değil. %s ekranındaki koda bakın.",
	[STR_R_NEED_T] = "KOD GEREKLİ",

	[STR_N_ASKING] =
		"%s bu bilgisayarı kontrol etmek istiyor. Kabul Et ya da İptal'e "
		"tıklayın.",
	[STR_N_REFUSED] = "%s içeri alınmadı.",
	[STR_N_GAVE_UP] = "%s istemekten vazgeçti.",
	[STR_N_WRONG] = "%s yanlış kod girdi.",
	[STR_N_CONTROLLING] = "%s şu an bu bilgisayarı kontrol ediyor.",
	[STR_N_STOPPED] = "%s bu bilgisayarın kontrolünü bıraktı.",

	[STR_H_TITLE] = "%s bu bilgisayarı kontrol etmek istiyor",
	[STR_H_TEXT] = "Ekranınızı görecek, fare ve klavyenizi kullanacak.",
	[STR_H_ACCEPT] = "Kabul Et",
	[STR_H_CANCEL] = "İptal",
	[STR_H_COUNTDOWN] = "Bu pencere %d saniye içinde kendiliğinden kapanır.",
	[STR_H_DISCONNECT] = "Bağlantıyı Kes",

	[STR_V_TITLE] =
		"%s kontrol ediliyor   -   durdurmak için bu pencereyi kapatın",
	[STR_V_MENU_CAD] = "Ctrl+Alt+Del Gönder   (ya da Ctrl+Alt+End)",
	[STR_V_REMOTE_CLOSED] = "Diğer bilgisayar bağlantıyı sonlandırdı.",

	[STR_I_NEEDS_CODE] =
		"-install bir kod ister, örneğin:  INWAY.exe -install -p 938572",
	[STR_I_PERMISSION] =
		"Windows şimdi izin isteyecek. \"Evet\" düğmesine tıklayın.",
	[STR_I_NO_PERMISSION] = "İzin verilmedi, hiçbir şey değiştirilmedi.",
	[STR_I_TITLE_INSTALL] = "ARKA PLAN PAYLAŞIMINI KUR",
	[STR_I_TITLE_REMOVE] = "ARKA PLAN PAYLAŞIMINI KALDIR",
	[STR_I_TITLE_OLD] = "ESKİ CONNECT'İ KALDIR",
	[STR_I_NO_SCM] = "Windows hizmet listesi açılamadı.",
	[STR_I_STOPPING_OLD] = "Eski arka plan paylaşımı durduruluyor ...",
	[STR_I_COPYING] = "INWAY şu klasöre kopyalanıyor: %s ...",
	[STR_I_COPY_FAIL] = "INWAY oraya kopyalanamadı.",
	[STR_I_SETUP] = "Arka plan paylaşımı ayarlanıyor ...",
	[STR_I_CREATE_FAIL] = "Hizmet oluşturulamadı (hata %lu).",
	[STR_I_FIREWALL] =
		"Windows güvenlik duvarında izin veriliyor (yalnızca bu ağ) ...",
	[STR_I_NOT_STARTED] =
		"Kuruldu ama başlamadı. Bilgisayarı yeniden başlatın.",
	[STR_I_STOPPING] = "Arka plan paylaşımı durduruluyor ...",
	[STR_I_NOT_INSTALLED] = "Arka plan paylaşımı zaten kurulu değil.",
	[STR_I_OLD_NOT_FOUND] = "Eski CONNECT bu bilgisayarda kurulu değil.",
	[STR_I_DONE_T] = "TAMAM",
	[STR_I_DONE] =
		"Bu bilgisayar artık her zaman paylaşılıyor; kilit ekranında ve "
		"yeniden başlattıktan sonra da.",
	[STR_I_REMOVE_HINT] = "Kaldırmak için:  INWAY.exe -uninstall",
	[STR_I_REMOVED] = "Arka plan paylaşımı kaldırıldı.",
	[STR_I_OLD_REMOVED] = "Eski CONNECT kaldırıldı.",
	[STR_I_TAKEOVER] =
		"INWAY bu bilgisayarda normal bir pencerede de açık mı? Kapatın: "
		"arka plan paylaşımı birkaç saniye içinde devralır.",
	[STR_I_PERMISSION_INSTALL] =
		"Windows şimdi yönetici izni soruyor. Evet: tam sunucu, kilit "
		"ekranında da. Hayır: oturum açıkken çalışan normal bir sunucu.",
	[STR_I_USER_SETUP] = "Oturum açılınca başlaması ayarlanıyor ...",
	[STR_I_USER_FAIL] = "Oturum açılınca başlaması ayarlanamadı.",
	[STR_I_USER_DONE] =
		"Bu bilgisayar artık oturum açıkken normal yetkiyle paylaşılıyor: "
		"yönetici pencereleri ve kilit ekranı kullanılamaz.",
	[STR_I_USER_UPGRADE] =
		"Tam sunucu için aynı -install komutunu tekrar çalıştırıp Evet'e "
		"tıklayın.",
	[STR_I_USER_REMOVED] = "Normal sunucu kaldırıldı.",

	[STR_PRESS_ENTER_CLOSE] = "Kapatmak için Enter'a basın.",
	[STR_PRESS_ENTER] = "Devam etmek için Enter'a basın.",
	[STR_E_P] = "-p 6 rakamlı bir kod ister, örneğin:  -p 938572",
	[STR_E_C] = "-c bir adres ya da ad ister, örneğin:  -c 2.253",
	[STR_E_UNKNOWN] = "Bilinmeyen seçenek: %s",
	[STR_E_NET] = "Ağ başlatılamadı.",
	[STR_HELP] =
		"  INWAY.exe                     paylaş ve başkalarına bağlan\n"
		"  INWAY.exe -p 938572           bu bilgisayar için bu kodu kullan\n"
		"  INWAY.exe -c 2.253 948248     2.253'e kodla bir kez bağlan\n"
		"  INWAY.exe -c 2.253            2.253'e bağlan, karşı taraf onaylar\n"
		"  INWAY.exe -c OFIS-PC 948248   bilgisayar adı da olur\n"
		"  INWAY.exe -install -p 938572  her zaman paylaş, kilit ekranında\n"
		"                                da (sunucular için)\n"
		"  INWAY.exe -install            aynısı, rastgele kodla (ya da\n"
		"                                sunucunun zaten olan koduyla)\n"
		"      ... -install -p 938572 -noupdate   otomatik güncellemesiz\n"
		"  INWAY.exe -uninstall          bunu geri kaldır\n"
		"  INWAY.exe -removeold          eski CONNECT aracını kaldır\n"
		"  INWAY.exe -lang en            dil: en ya da tr\n"
		"  INWAY.exe -noupdate           yeni sürüm arama\n"
		"\n"
		"  INWAY'i yalnızca size ait ya da kontrol etme izniniz olan\n"
		"  bilgisayarlarda kullanın. Bkz. DISCLAIMER.md.\n",
	[STR_U_FOUND] = "Yeni INWAY çıktı (%s). Güncelleniyor ...",
	[STR_U_FAILED] = "Güncelleme olmadı. Bu sürümle devam ediliyor.",
	[STR_U_DONE] = "INWAY %s sürümüne güncellendi.",
	[STR_C_SENT] = "Kopyaladığınız dosyalar %s üzerinde yapıştırılmaya hazır.",
	[STR_C_GOT] =
		"%s üzerinde kopyalanan dosyalar burada yapıştırılmaya hazır.",
	[STR_C_TOO_BIG] = "Bu dosyalar 4 GB'tan büyük, %s tarafına aktarılamaz.",
	[STR_C_CANT_READ] =
		"Dosyalar kopyalandı ama INWAY onları %s tarafına göndermek için "
		"okuyamadı.",
	[STR_N_ADMIN_ASK] =
		"Windows şimdi %s için yönetici izni soruyor. Evet: tam kontrol. "
		"Hayır: normal yetki.",
	[STR_N_ADMIN_NO] =
		"%s normal yetkiyle bağlı: yönetici pencereleri kullanılamaz.",
	[STR_SVC_USER_HINT] = "Oturum açıkken hep açık.",
	[STR_UPDATING_WAIT] =
		"%s önce INWAY'i güncelliyor. Birazdan yeniden bağlanılıyor ...",
	[STR_R_UPDATING_T] = "GÜNCELLENİYOR",
	[STR_R_UPDATING] =
		"%s hâlâ INWAY'i güncelliyor. Bir dakika sonra tekrar deneyin.",
	[STR_NAME] = "Ad",
	[STR_LOOKING] = "%s bu ağda aranıyor ...",
	[STR_NO_SUCH_NAME] =
		"Bu ağda %s adında bir bilgisayar bulunamadı. Adresini deneyin."
};
/*   /\   #==========================================================#   /\   */
/*  //\\  |                         TURKISH                          |  //\\  */
/* '""""' #==========================================================# '""""' */

static const char	*const *G__LANG__ = G__EN__;

void
	LANG_INIT(const char *FORCE)
{
	LANGID	ID;

	if (FORCE && (FORCE[0] == 't' || FORCE[0] == 'T'))
	{
		G__LANG__ = G__TR__;
		return ;
	}
	if (FORCE && (FORCE[0] == 'e' || FORCE[0] == 'E'))
	{
		G__LANG__ = G__EN__;
		return ;
	}
	ID = GetUserDefaultUILanguage();
	G__LANG__ = (PRIMARYLANGID(ID) == LANG_TURKISH) ? G__TR__ : G__EN__;
}

const char
	*LANG(enum E_STR ID)
{
	if ((unsigned)ID >= STR__COUNT)
	{
		return ("");
	}
	if (G__LANG__[ID])
	{
		return (G__LANG__[ID]);
	}
	return (G__EN__[ID] ? G__EN__[ID] : "");
}

/* "EN" OR "TR": PASSED ON TO HELPERS WITH -LANG */
const char
	*LANG_NAME(void)
{
	return (G__LANG__ == G__TR__ ? "tr" : "en");
}
