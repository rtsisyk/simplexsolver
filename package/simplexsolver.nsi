!define PRODUCT_NAME "SimplexSolver"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_VERSION_FULL "1.0.1.0"
!define PRODUCT_PUBLISHER 'Roman Tsisyk'
!define PRODUCT_WEB_SITE "http://roman.tsisyk.com/projects/simplexsolver"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\SimplexSolver.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

!define PROJECT_DIR "..\"
!define RELEASE_DIR "${PROJECT_DIR}\release"
!define PACKAGE_DIR "${PROJECT_DIR}\package"

SetCompressor /SOLID /FINAL lzma 

!include "MUI.nsh"

RequestExecutionLevel user 

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
BrandingText '${PRODUCT_PUBLISHER}'
OutFile "${PACKAGE_DIR}\SimplexSolver-${PRODUCT_VERSION}-setup.exe"
InstallDir "$PROGRAMFILES\SimplexSolver"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show
CRCCheck on
XPStyle On


Function ExecAppFile
	ExecShell "" "$INSTDIR\SimplexSolver.exe"
    /* UAC::Exec '' '"$INSTDIR\SimplexSolver.exe"' '' '$INSTDIR' */
FunctionEnd
 

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_LANGDLL_ALLLANGUAGES
Icon "${PROJECT_DIR}\gui\icon\main.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${PACKAGE_DIR}\nsis-wizard.bmp"
!define MUI_ICON "${PROJECT_DIR}\gui\icon\main.ico"
!define MUI_UNICON "${PROJECT_DIR}\gui\icon\main.ico"

; Welcome page
!define MUI_COMPONENTSPAGE_NODESC
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "${PROJECT_DIR}\LICENSE"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION ExecAppFile
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
;!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH


!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"

!insertmacro MUI_RESERVEFILE_LANGDLL
; MUI end ------

VIAddVersionKey /LANG=${LANG_RUSSIAN} "ProductName" '${PRODUCT_NAME}'
VIAddVersionKey /LANG=${LANG_RUSSIAN} "FileDescription" '${PRODUCT_NAME}'
VIAddVersionKey /LANG=${LANG_RUSSIAN} "CompanyName" '${PRODUCT_PUBLISHER}'
VIAddVersionKey /LANG=${LANG_RUSSIAN} "LegalCopyright" '© ${PRODUCT_PUBLISHER}'
VIAddVersionKey /LANG=${LANG_RUSSIAN} "FileVersion" '${PRODUCT_VERSION}'
VIAddVersionKey /LANG=${LANG_RUSSIAN} "ProductVersion" '${PRODUCT_VERSION}'

VIProductVersion ${PRODUCT_VERSION_FULL}
/* 
Var WINVER
!macro GetAdmin
  UAC_Elevate:
    UAC::RunElevated 
    StrCmp 1223 $0 UAC_ElevationAborted ; UAC dialog aborted by user?
    StrCmp 0 $0 0 UAC_Err ; Error?
    StrCmp 1 $1 0 UAC_Success ;Are we the real deal or just the wrapper?
    Quit
 
  UAC_Err:
    MessageBox mb_iconstop "Administrator rights required for install!."
    Abort
 
  UAC_ElevationAborted:
    # elevation was aborted, run as normal?
    MessageBox mb_iconstop "Administrator rights required for install!"
    Abort
 
  UAC_Success:
    StrCmp 1 $3 +4 ;Admin?
    StrCmp 3 $1 0 UAC_ElevationAborted ;Try again?
    MessageBox mb_iconstop "Administrator rights required for install!"
    goto UAC_Elevate 
!macroend

!macro CheckNtVersionString
  StrCpy $R1 $R0 3
  StrCmp $R1 '5.0' lbl_winnt_2000
  StrCmp $R1 '5.1' lbl_winnt_XP
  StrCmp $R1 '5.2' lbl_winnt_2003
  StrCmp $R1 '6.0' lbl_winnt_vista
  StrCmp $R1 '6.1' lbl_winnt_seven

  Goto lbl_error

  lbl_winnt_2000:
    Strcpy $WINVER '2000'
    Goto lbl_done

  lbl_winnt_XP:
    Strcpy $WINVER 'XP'
    Goto lbl_done

  lbl_winnt_2003:
    Strcpy $WINVER '2003'
    Goto lbl_done

  lbl_winnt_vista:
    Strcpy $WINVER 'VISTA'

  lbl_winnt_seven:
    Strcpy $WINVER 'SEVEN'
!macroend
*/

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  
  ;!insertmacro GetAdmin
  Push $R0
  Push $R1
  ClearErrors

  ReadRegStr $R0 HKLM \
  "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion

  IfErrors lbl_error 0

  ; not needed, should work on any version of WinNT
  ;!CheckNtVersionString

  Goto lbl_done

  lbl_error:
    Abort "Your version of Windows is not supported!"
    Strcpy $R0 ''

  lbl_done:

  Pop $R1
  Pop $R0

FunctionEnd

Section "!SimplexSolver"
  SectionIN RO

  SetOutPath "$INSTDIR"
  File "${RELEASE_DIR}\SimplexSolver.exe"
  File "${PROJECT_DIR}\LICENSE"

  SetOutPath "$INSTDIR\locale"
  ; Install all locales
  File "${PROJECT_DIR}\locale\*.qm"

  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\SimplexSolver.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" '$(^Name)'
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" '$INSTDIR\uninst.exe'
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" '$INSTDIR\SimplexSolver.exe'
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" '${PRODUCT_VERSION}'
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" '${PRODUCT_WEB_SITE}'
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" '${PRODUCT_PUBLISHER}'

SectionEnd

/*
Section "VC Runtime"
;
;Download from here http://www.microsoft.com/downloads/en/details.aspx?familyid=a5c84275-3b97-4ab7-a40d-3802b2af5fc2&displaylang=en
;

 SetOutPath "$TEMP"
SetOverwrite ifdiff
 File "${PACKAGE_DIR}\vcredist_x86.exe"
 ExecWait '$TEMP\vcredist_x86.exe /q'
SectionEnd

Section "Qt Runtime"
SetOutPath "$INSTDIR"
 File "${QTBIN_DIR}\QtCore4.dll"
 File "${QTBIN_DIR}\QtGui4.dll"
SectionEnd
*/

Section /o "Manual (in Russian)" 
  SetOutPath "$INSTDIR"
  File "${PROJECT_DIR}\doc\manual\manual.pdf"
SectionEnd

Section "Start menu links"
  SetOutPath "$INSTDIR"
  ;SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\SimplexSolver"
  CreateShortCut "$SMPROGRAMS\SimplexSolver\SimplexSolver.lnk" "$INSTDIR\SimplexSolver.exe"
  CreateShortCut "$SMPROGRAMS\SimplexSolver\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section "Desktop links"
  SetOutPath "$INSTDIR"
  ;SetShellVarContext all
  CreateShortCut "$DESKTOP\SimplexSolver.lnk" "$INSTDIR\SimplexSolver.exe"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
 ; !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} ""
!insertmacro MUI_FUNCTION_DESCRIPTION_END

/*
Function .OnInstFailed
    UAC::Unload ;Must call unload!
FunctionEnd
 
Function .OnInstSuccess
    UAC::Unload ;Must call unload!
FunctionEnd

Function un.onUninstSuccess
    UAC::Unload ;Must call unload!
FunctionEnd

Function un.onUninstFailed
    UAC::Unload ;Must call unload!
FunctionEnd

Function un.onInit
  !insertmacro GetAdmin
FunctionEnd
*/

Section Uninstall
  SetOutPath "$TEMP"

  Delete "$INSTDIR\uninst.exe"   

  Delete "$INSTDIR\SimplexSolver.exe"

;  Delete "$INSTDIR\QtCore4.dll"
;  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\manual.pdf"
  Delete "$INSTDIR\locale\*.qm"

  RMDir /r "$INSTDIR\locale" 
  RMDir /r "$INSTDIR"
 
  ;SetShellVarContext all
  Delete "$DESKTOP\SimplexSolver.lnk"

  Delete "$SMPROGRAMS\SimplexSolver\SimplexSolver.lnk"
  Delete "$SMPROGRAMS\SimplexSolver\Uninstall.lnk"
  RMDir /r "$SMPROGRAMS\SimplexSolver"
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose false
SectionEnd
