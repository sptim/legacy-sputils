REM Extract Window Message IDs from Toolkit include file pmwin.h

REM WM_* (Window Messages)
REM EM_* (Entryfield Messages)
REM SBM_* (Scrollbar Messages)
REM STM_*
REM BM_* (Button Messages)
REM SM_ (Static Messages)
REM DM_ (Direct manipulation Messages/Drag Messages)
REM MM_ (Menu Control Messages)
REM CBM_ (Combination box Messages)
REM LM_ (Listbox Messages)
REM TM_ (Titlebar Messages)

SET LANG=de_DE
PERL EXTRACT_WINMSG_CONV1.PL PMWIN.H | SORT | PERL EXTRACT_WINMSG_CONV2.PL >messages.h'
