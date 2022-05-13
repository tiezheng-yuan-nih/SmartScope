// Master list of script commands

// These are macros defined differently to create the enum of command codes, the
// function declarations, and the command table.  In every case, the first entry is the
// MixedCase command name, and the last entry is the upper case of that.
//
// The two numbers are the minimum arguments and a set of flags.  The first two bits of
// the flags affect whether arithmetic is allowed.
// Arithmetic is allowed for any command with bit 1 set in the second number (1), and for
// any "Set" command that does not have bit 2 set there (2).  Starting with % lists all
// explicitly allowed commands.  It is redundant to set bit 1 for Set command
// Bit 3 (4) signifies that looping in OnIdle is allowed for a command, it is redundant
// to set this for a Report command.
// Bit 4 (8) indicates that a command is NOT available in external scripting and it is
// added in the macros that construct, so not specified here
// In sum:
// # of args, 1 for arith allowed + 2 for not allowed in Set... + 4 looping in OnIdle OK
//
// MAC_SAME_NAME is used when the command matches the function name, so the function name
// is not needed
// MAC_DIFF_NAME is used when the function had to be named differently from the command
// so that function name is included and is declared in MacroProcessor.h
// MAC_SAME_FUNC is used for a command that calls the same function as some other command,
// for which the function has already been declared, so it also includes the function name
//
// To support external scripting, each of these strings will be followed by _NOARG or _ARG
// for commands available externally.  _NOARG are ones that take no arguments, and _ARG
// ones have a final macro entry, a set of keys for the arguments that the command
// currently takes. These keys letters are:
// D, S, or I for double, string, or int, then d, s, or i for optional doubles, strings,
// or ints.  Python will do type checking, so when in doubt, use D/d instead of I/i.
// Plugin functions can either be constructed directly from the macros with these keys
// stringified and interpreted, or functions can be constructed by a script.
//
// Real commands that can be included in a SerialeEM script start with Exit - keep that
// as the first such one!
//
// New commands must be added at the end or they will break existing Python modules, 
// new Python-only commands need to be added to pythonOnlyCmds in ::CMacroProcessor
//
// The longest command name is now 25 characters but 23 is a more common limit

MAC_SAME_NAME(ScriptEnd, 0, 0, SCRIPTEND)
MAC_DIFF_NAME(Label, 0, 0, NoOperation, LABEL)
MAC_DIFF_NAME_ARG(SetVariable, 0, 0, SetVariableCmd, SETVARIABLE, SS)
MAC_SAME_FUNC_ARG(SetPersistentVar, 0, 0, SetVariableCmd, SETPERSISTENTVAR, SS)
MAC_SAME_NAME_ARG(GetVariable, 1, 4, GETVARIABLE, S)
MAC_SAME_NAME(SetStringVar, 0, 0, SETSTRINGVAR)
MAC_SAME_FUNC(DoKeyBreak, 0, 0, Break, DOKEYBREAK)
MAC_SAME_FUNC(ZeroLoopElseIf, 0, 0, If, ZEROLOOPELSEIF)
MAC_SAME_FUNC_ARG(Exit, 0, 4, ScriptEnd, EXIT, ds)
MAC_SAME_FUNC_ARG(Return, 0, 4, ScriptEnd, RETURN, ssssss)
MAC_SAME_FUNC(Function, 1, 4, ScriptEnd, FUNCTION)
MAC_SAME_FUNC(EndFunction, 0, 4, ScriptEnd, ENDFUNCTION)
MAC_SAME_NAME(Repeat, 0, 4, REPEAT)
MAC_SAME_FUNC(MacroName, 1, 4, NoOperation, MACRONAME)
MAC_SAME_FUNC(LongName, 1, 4, NoOperation, LONGNAME)
MAC_SAME_NAME_ARG(KeyBreak, 0, 4, KEYBREAK, s)
MAC_SAME_FUNC(Require, 1, 4, NoOperation, REQUIRE)
MAC_SAME_FUNC(ScriptName, 1, 4, NoOperation, SCRIPTNAME)
MAC_SAME_FUNC(ReadOnlyUnlessAdmin, 0, 4, NoOperation, READONLYUNLESSADMIN)
MAC_SAME_NAME(EndLoop, 0, 4, ENDLOOP)
MAC_SAME_NAME(Loop, 1, 5, LOOP)
MAC_SAME_FUNC(DoLoop, 3, 5, Loop, DOLOOP)
MAC_SAME_NAME(If, 3, 4, IF)
MAC_SAME_NAME(Endif, 0, 4, ENDIF)
MAC_SAME_NAME(Else, 0, 4, ELSE)
MAC_SAME_FUNC(ElseIf, 3, 4, Else, ELSEIF)
MAC_SAME_NAME(Break, 0, 4, BREAK)
MAC_SAME_FUNC(Continue, 0, 4, Break, CONTINUE)
MAC_SAME_NAME(Try, 0, 4, TRY)
MAC_SAME_NAME(Catch, 0, 4, CATCH)
MAC_SAME_NAME(EndTry, 0, 4, ENDTRY)
MAC_SAME_NAME(Throw, 0, 4, THROW)
MAC_SAME_NAME(SkipTo, 1, 4, SKIPTO)
MAC_SAME_NAME(DoMacro, 1, 4, DOMACRO)
MAC_SAME_FUNC_ARG(Call, 1, 4, DoMacro, CALL, S)
MAC_SAME_FUNC_ARG(CallMacro, 1, 4, DoMacro, CALLMACRO, I)
MAC_SAME_FUNC_ARG(CallFunction, 1, 4, DoMacro, CALLFUNCTION, Ssddddddddddddddddd)
MAC_SAME_FUNC(DoScript, 1, 4, DoMacro, DOSCRIPT)
MAC_SAME_FUNC_ARG(CallScript, 1, 4, DoMacro, CALLSCRIPT, I)
MAC_SAME_FUNC_ARG(CallStringArray, 1, 4, DoMacro, CALLSTRINGARRAY, Si)
MAC_SAME_NAME(ParseQuotedStrings, 0, 4, PARSEQUOTEDSTRINGS)
MAC_SAME_NAME(StringArrayToScript, 1, 4, STRINGARRAYTOSCRIPT)
MAC_SAME_NAME(OnStopCallFunc, 1, 4, ONSTOPCALLFUNC)
MAC_SAME_NAME_ARG(NoMessageBoxOnError, 0, 4, NOMESSAGEBOXONERROR, i)
MAC_SAME_NAME(Test, 1, 4, TEST)
MAC_SAME_FUNC(AppendToArray, 2, 4, SetVariableCmd, APPENDTOARRAY)
MAC_SAME_NAME(TruncateArray, 2, 4, TRUNCATEARRAY)
MAC_SAME_NAME_ARG(ArrayStatistics, 1, 4, ARRAYSTATISTICS, S)
MAC_SAME_NAME_ARG(LinearFitToVars, 2, 4, LINEARFITTOVARS, SSs)
MAC_SAME_NAME_NOARG(ListVars, 0, 4 , LISTVARS)
MAC_SAME_NAME_NOARG(ListPersistentVars, 0, 4 , LISTPERSISTENTVARS)
MAC_SAME_NAME_NOARG(ClearPersistentVars, 0, 4, CLEARPERSISTENTVARS)
MAC_SAME_NAME_ARG(MakeVarPersistent, 1, 4, MAKEVARPERSISTENT, Si)
MAC_SAME_NAME_ARG(IsVariableDefined, 1, 4, ISVARIABLEDEFINED, S)
MAC_DIFF_NAME(NewArray, 3, 4, NewArrayCmd, NEWARRAY)
MAC_SAME_FUNC(New2DArray, 1, 4, NewArrayCmd, NEW2DARRAY)
MAC_SAME_NAME(LocalVar, 1, 4, LOCALVAR)
MAC_SAME_NAME(LocalLoopIndexes, 0, 4, LOCALLOOPINDEXES)
MAC_SAME_NAME(Plugin, 2, 0, PLUGIN)
MAC_SAME_NAME_NOARG(ListPluginCalls, 0, 4, LISTPLUGINCALLS)
MAC_SAME_NAME_ARG(FlashDisplay, 0, 0, FLASHDISPLAY, ids)
MAC_SAME_NAME_ARG(TiltUp, 0, 0, TILTUP, idd)
MAC_SAME_FUNC_ARG(U, 0, 0, TiltUp, U, idd)
MAC_SAME_NAME_ARG(TiltDown, 0, 0, TILTDOWN, idd)
MAC_SAME_FUNC_ARG(D, 0, 0, TiltDown, D, idd)
MAC_SAME_NAME_ARG(TiltTo, 1, 1, TILTTO, Didd)
MAC_SAME_FUNC_ARG(TiltBy, 1, 1, TiltTo, TILTBY, Didd)
MAC_SAME_NAME_ARG(SetStageBAxis, 1, 1, SETSTAGEBAXIS, D)
MAC_SAME_NAME_NOARG(OpenDECameraCover, 0, 0, OPENDECAMERACOVER)
MAC_SAME_NAME_NOARG(View, 0, 0, VIEW)
MAC_SAME_FUNC_NOARG(V, 0, 0, View, V)
MAC_SAME_NAME_NOARG(Focus, 0, 0, FOCUS)
MAC_SAME_FUNC_NOARG(F, 0, 0, Focus, F)
MAC_SAME_NAME_NOARG(Trial, 0, 0, TRIAL)
MAC_SAME_FUNC_NOARG(T, 0, 0, Trial, T)
MAC_SAME_NAME_NOARG(Record, 0, 0, RECORD)
MAC_SAME_FUNC_NOARG(R, 0, 0, Record, R)
MAC_SAME_NAME_NOARG(Preview, 0, 0, PREVIEW)
MAC_SAME_FUNC_NOARG(L, 0, 0, Preview, L)
MAC_SAME_NAME_NOARG(Search, 0, 0, SEARCH)
MAC_SAME_NAME_NOARG(Montage, 0, 0, MONTAGE)
MAC_SAME_FUNC_NOARG(M, 0, 0, Montage, M)
MAC_SAME_FUNC_ARG(PreCookMontage, 1, 1, Montage, PRECOOKMONTAGE, Diid)
MAC_SAME_NAME_NOARG(OppositeTrial, 0, 0, OPPOSITETRIAL)
MAC_SAME_FUNC_NOARG(OppositeFocus, 0, 0, OppositeTrial, OPPOSITEFOCUS)
MAC_SAME_FUNC_ARG(OppositeAutoFocus, 0, 0, OppositeTrial, OPPOSITEAUTOFOCUS, i)
MAC_SAME_NAME_ARG(AcquireToMatchBuffer, 1, 0, ACQUIRETOMATCHBUFFER, S)
MAC_SAME_NAME_ARG(StepFocusNextShot, 2, 5, STEPFOCUSNEXTSHOT, DDdd)
MAC_SAME_NAME_ARG(SmoothFocusNextShot, 2, 5, SMOOTHFOCUSNEXTSHOT, DD)
MAC_SAME_NAME_NOARG(DeferStackingNextShot, 0, 4, DEFERSTACKINGNEXTSHOT)
MAC_SAME_NAME_ARG(EarlyReturnNextShot, 1, 5, EARLYRETURNNEXTSHOT, Ii)
MAC_SAME_NAME_NOARG(GetDeferredSum, 0, 0, GETDEFERREDSUM)
MAC_SAME_NAME_ARG(FrameThresholdNextShot, 1, 5, FRAMETHRESHOLDNEXTSHOT, Dddd)
MAC_SAME_NAME_ARG(QueueFrameTiltSeries, 3, 4, QUEUEFRAMETILTSERIES, DDIdddddd)
MAC_SAME_FUNC_ARG(FrameSeriesFromVar, 2, 4, QueueFrameTiltSeries, FRAMESERIESFROMVAR, SId)
MAC_SAME_NAME_ARG(SetFrameSeriesParams, 1, 4, SETFRAMESERIESPARAMS, Idddii)
MAC_SAME_NAME_ARG(WriteFrameSeriesAngles, 1, 4, WRITEFRAMESERIESANGLES, S)
MAC_SAME_NAME_NOARG(ReportFrameTiltSum, 0, 0, REPORTFRAMETILTSUM)
MAC_SAME_NAME_ARG(ModifyFrameTSShifts, 3, 5, MODIFYFRAMETSSHIFTS, IDD)
MAC_SAME_NAME_ARG(ReplaceFrameTSFocus, 1, 4, REPLACEFRAMETSFOCUS, S)
MAC_SAME_FUNC_ARG(ReplaceFrameTSShifts, 2, 4, ReplaceFrameTSFocus, REPLACEFRAMETSSHIFTS, SS)
MAC_SAME_NAME_NOARG(RetractCamera, 0, 0, RETRACTCAMERA)
MAC_SAME_NAME_ARG(RecordAndTiltUp, 0, 0, RECORDANDTILTUP, idd)
MAC_SAME_FUNC_ARG(RecordAndTiltDown, 0, 0, RecordAndTiltUp, RECORDANDTILTDOWN, idd)
MAC_SAME_FUNC_ARG(RecordAndTiltTo, 1, 1, RecordAndTiltUp, RECORDANDTILTTO, Ddidd)
MAC_SAME_NAME_NOARG(ArePostActionsEnabled, 0, 4, AREPOSTACTIONSENABLED)
MAC_SAME_NAME_ARG(TiltDuringRecord, 2, 1, TILTDURINGRECORD, DDd)
MAC_SAME_NAME_ARG(SetDoseAdjustmentFactor, 1, 0, SETDOSEADJUSTMENTFACTOR, D)
MAC_SAME_NAME_ARG(TestNextMultiShot, 1, 0, TESTNEXTMULTISHOT, I)
MAC_SAME_NAME_ARG(MultipleRecords, 0, 1, MULTIPLERECORDS, iiddiiiiiiid)
MAC_SAME_NAME_ARG(RotateMultiShotPattern, 1, 4, ROTATEMULTISHOTPATTERN, Di)
MAC_SAME_NAME_NOARG(AutoAlign, 0, 0, AUTOALIGN)
MAC_SAME_FUNC_NOARG(A, 0, 0, AutoAlign, A)
MAC_SAME_FUNC_ARG(AlignTo, 1, 0, AutoAlign, ALIGNTO, Sii)
MAC_SAME_FUNC_ARG(ConicalAlignTo, 2, 0, AutoAlign, CONICALALIGNTO, SDi)
MAC_SAME_NAME_ARG(LimitNextAutoAlign, 1, 5, LIMITNEXTAUTOALIGN, D)
MAC_SAME_NAME_ARG(AlignWithRotation, 3, 0, ALIGNWITHROTATION, SDD)
MAC_SAME_NAME_ARG(AutoFocus, 0, 0, AUTOFOCUS, ii)
MAC_SAME_FUNC_ARG(G, 0, 0, AutoFocus, G, ii)
MAC_SAME_NAME_ARG(BeamTiltDirection, 1, 5, BEAMTILTDIRECTION, I)
MAC_SAME_NAME_ARG(FocusChangeLimits, 2, 5, FOCUSCHANGELIMITS, DD)
MAC_SAME_NAME_ARG(AbsoluteFocusLimits, 2, 5, ABSOLUTEFOCUSLIMITS, DD)
MAC_SAME_NAME_ARG(CorrectAstigmatism, 0, 0, CORRECTASTIGMATISM, i)
MAC_SAME_NAME_ARG(CorrectComa, 0, 0, CORRECTCOMA, i)
MAC_SAME_NAME_ARG(ZemlinTableau, 1, 1, ZEMLINTABLEAU, Dii)
MAC_SAME_NAME_ARG(CBAstigComa, 3, 0, CBASTIGCOMA, IIIii)
MAC_SAME_NAME_ARG(FixAstigmatismByCTF, 0, 0, FIXASTIGMATISMBYCTF, iii)
MAC_SAME_FUNC_ARG(FixComaByCTF, 0, 0, FixAstigmatismByCTF, FIXCOMABYCTF, iiii)
MAC_SAME_NAME_NOARG(ReportStigmatorNeeded, 0, 0, REPORTSTIGMATORNEEDED)
MAC_SAME_NAME_NOARG(ReportComaTiltNeeded, 0, 0, REPORTCOMATILTNEEDED)
MAC_SAME_NAME_NOARG(ReportComaVsISmatrix, 0, 0, REPORTCOMAVSISMATRIX)
MAC_SAME_NAME_ARG(Save, 0, 0, SAVE, si)
MAC_SAME_FUNC_ARG(S, 0, 0, Save, S, si)
MAC_SAME_NAME_ARG(ReadFile, 1, 0, READFILE, Is)
MAC_SAME_NAME_ARG(ReadOtherFile, 3, 4, READOTHERFILE, ISS)
MAC_SAME_NAME_ARG(RetryReadOtherFile, 1, 4, RETRYREADOTHERFILE, I)
MAC_SAME_NAME_ARG(SaveToOtherFile, 4, 4, SAVETOOTHERFILE, SSSS)
MAC_SAME_FUNC_ARG(SnapshotToFile, 6, 0, SaveToOtherFile, SNAPSHOTTOFILE, DDSSSS)
MAC_SAME_NAME_ARG(OpenNewFile, 1, 4, OPENNEWFILE, S)
MAC_SAME_FUNC_ARG(OpenNewMontage, 3, 4, OpenNewFile, OPENNEWMONTAGE, IIS)
MAC_SAME_FUNC_ARG(OpenFrameSumFile, 1, 4, OpenNewFile, OPENFRAMESUMFILE, S)
MAC_SAME_NAME_ARG(SetupWaffleMontage, 2, 6, SETUPWAFFLEMONTAGE, DS)
MAC_SAME_NAME_NOARG(ReportNumMontagePieces, 0, 4, REPORTNUMMONTAGEPIECES)
MAC_SAME_NAME_ARG(EnterNameOpenFile, 0, 4, ENTERNAMEOPENFILE, s)
MAC_SAME_NAME_ARG(ChooserForNewFile, 2, 4, CHOOSERFORNEWFILE, IS)
MAC_SAME_NAME_ARG(ReadTextFile, 2, 4, READTEXTFILE, SS)
MAC_SAME_FUNC_ARG(Read2DTextFile, 2, 4, ReadTextFile, READ2DTEXTFILE, SS)
MAC_SAME_FUNC_ARG(ReadStringsFromFile, 2, 4, ReadTextFile, READSTRINGSFROMFILE, SS)
MAC_SAME_NAME_ARG(OpenTextFile, 4, 4, OPENTEXTFILE, SSIS)
MAC_SAME_NAME_ARG(WriteLineToFile, 1, 4, WRITELINETOFILE, Ss)
MAC_SAME_NAME_ARG(ReadLineToArray, 2, 4, READLINETOARRAY, SS)
MAC_SAME_FUNC_ARG(ReadLineToString, 2, 4, ReadLineToArray, READLINETOSTRING, SS)
MAC_SAME_NAME_ARG(CloseTextFile, 1, 4, CLOSETEXTFILE, S)
MAC_SAME_NAME_ARG(FlushTextFile, 1, 4, FLUSHTEXTFILE, S)
MAC_SAME_NAME_ARG(IsTextFileOpen, 1, 4, ISTEXTFILEOPEN, Ss)
MAC_SAME_NAME_ARG(UserSetDirectory, 0, 4, USERSETDIRECTORY, s)
MAC_SAME_NAME_NOARG(OpenChooserInCurrentDir, 0, 0, OPENCHOOSERINCURRENTDIR)
MAC_SAME_NAME_ARG(SetNewFileType, 1, 4, SETNEWFILETYPE, Ii)
MAC_SAME_NAME_ARG(OpenOldFile, 1, 4, OPENOLDFILE, S)
MAC_SAME_NAME_NOARG(UserOpenOldFile, 0, 0, USEROPENOLDFILE)
MAC_SAME_NAME_ARG(CloseFile, 0, 4, CLOSEFILE, s)
MAC_SAME_NAME_ARG(RemoveFile, 1, 4, REMOVEFILE, S)
MAC_SAME_NAME_ARG(ReportCurrentFilename, 0, 4, REPORTCURRENTFILENAME, i)
MAC_SAME_FUNC_NOARG(ReportLastFrameFile, 0, 4, ReportCurrentFilename, REPORTLASTFRAMEFILE)
MAC_SAME_FUNC_ARG(ReportNavFile, 0, 0, ReportCurrentFilename, REPORTNAVFILE, i)
MAC_SAME_NAME_NOARG(ReportFrameBaseName, 0, 0, REPORTFRAMEBASENAME)
MAC_SAME_NAME_ARG(GetFileInWatchedDir, 1, 4, GETFILEINWATCHEDDIR, S)
MAC_SAME_FUNC_ARG(RunScriptInWatchedDir, 1, 4, GetFileInWatchedDir, RUNSCRIPTINWATCHEDDIR, S)
MAC_SAME_NAME_ARG(AllowFileOverwrite, 1, 4, ALLOWFILEOVERWRITE, I)
MAC_SAME_NAME_ARG(SetDirectory, 1, 6, SETDIRECTORY, S)
MAC_SAME_FUNC_ARG(MakeDirectory, 1, 4, SetDirectory, MAKEDIRECTORY, S)
MAC_SAME_NAME_NOARG(ReportDirectory, 0, 0, REPORTDIRECTORY)
MAC_SAME_NAME_ARG(DoesFileExist, 1, 0, DOESFILEEXIST, S)
MAC_SAME_NAME_NOARG(MakeDateTimeDir, 0, 4, MAKEDATETIMEDIR)
MAC_SAME_NAME_ARG(SwitchToFile, 1, 4, SWITCHTOFILE, I)
MAC_SAME_NAME_NOARG(ReportFileNumber, 0, 0, REPORTFILENUMBER)
MAC_SAME_NAME_ARG(AddToAutodoc, 2, 4, ADDTOAUTODOC, SS)
MAC_SAME_FUNC_NOARG(WriteAutodoc, 0, 4, AddToAutodoc, WRITEAUTODOC)
MAC_SAME_NAME_ARG(AddToFrameMdoc, 2, 4, ADDTOFRAMEMDOC, SS)
MAC_SAME_FUNC_NOARG(WriteFrameMdoc, 0, 4, AddToFrameMdoc, WRITEFRAMEMDOC)
MAC_SAME_NAME_NOARG(ReportFrameMdocOpen, 0, 4, REPORTFRAMEMDOCOPEN)
MAC_SAME_NAME_NOARG(DeferWritingFrameMdoc, 0, 4, DEFERWRITINGFRAMEMDOC)
MAC_SAME_NAME_ARG(OpenFrameMdoc, 1, 4, OPENFRAMEMDOC, S)
MAC_SAME_NAME_ARG(CloseFrameMdoc, 0, 4, CLOSEFRAMEMDOC, i)
MAC_SAME_NAME_ARG(AddToNextFrameStackMdoc, 2, 4, ADDTONEXTFRAMESTACKMDOC, SS)
MAC_SAME_FUNC_ARG(StartNextFrameStackMdoc, 2, 4, AddToNextFrameStackMdoc, STARTNEXTFRAMESTACKMDOC, SS)
MAC_SAME_NAME_ARG(AlignWholeTSOnly, 0, 4, ALIGNWHOLETSONLY, i)
MAC_SAME_NAME_NOARG(WriteComForTSAlign, 0, 4, WRITECOMFORTSALIGN)
MAC_SAME_NAME_ARG(SaveLogOpenNew, 0, 4, SAVELOGOPENNEW, s)
MAC_SAME_FUNC_ARG(CloseLogOpenNew, 0, 4, SaveLogOpenNew, CLOSELOGOPENNEW, i)
MAC_SAME_NAME_ARG(SaveLog, 0, 4, SAVELOG, is)
MAC_SAME_NAME_ARG(DeferLogUpdates, 0, 4, DEFERLOGUPDATES, i)
MAC_SAME_NAME_NOARG(SaveCalibrations, 0, 4, SAVECALIBRATIONS)
MAC_SAME_NAME_ARG(SetProperty, 2, 4, SETPROPERTY, SI)
MAC_SAME_NAME_ARG(ReportUserSetting, 1, 0, REPORTUSERSETTING, S)
MAC_SAME_FUNC_ARG(ReportProperty, 1, 0, ReportUserSetting, REPORTPROPERTY, S)
MAC_SAME_NAME_ARG(SetUserSetting, 2, 4, SETUSERSETTING, SDi)
MAC_SAME_NAME_ARG(Copy, 2, 0, COPY, SS)
MAC_SAME_NAME_ARG(Show, 1, 0, SHOW, S)
MAC_SAME_NAME_ARG(ChangeFocus, 1, 1, CHANGEFOCUS, D)
MAC_SAME_NAME_ARG(SetDefocus, 1, 0, SETDEFOCUS, D)
MAC_SAME_NAME_ARG(SetStandardFocus, 1, 4, SETSTANDARDFOCUS, D)
MAC_SAME_FUNC_ARG(SetAbsoluteFocus, 1, 0, SetStandardFocus, SETABSOLUTEFOCUS, D)
MAC_SAME_NAME_NOARG(SetEucentricFocus, 0, 4, SETEUCENTRICFOCUS)
MAC_SAME_NAME_NOARG(CalEucentricFocus, 0, 4, CALEUCENTRICFOCUS)
MAC_SAME_NAME_ARG(IncTargetDefocus, 1, 5, INCTARGETDEFOCUS, D)
MAC_SAME_NAME_ARG(SetTargetDefocus, 1, 4, SETTARGETDEFOCUS, D)
MAC_SAME_NAME_NOARG(ReportAutofocusOffset, 0, 0, REPORTAUTOFOCUSOFFSET)
MAC_SAME_NAME_ARG(SetAutofocusOffset, 1, 4, SETAUTOFOCUSOFFSET, D)
MAC_SAME_NAME_ARG(SetObjFocus, 1, 0, SETOBJFOCUS, D)
MAC_SAME_NAME_ARG(SetDiffractionFocus, 1, 0, SETDIFFRACTIONFOCUS, D)
MAC_SAME_NAME_NOARG(ResetDefocus, 0, 0, RESETDEFOCUS)
MAC_SAME_NAME_ARG(SetMag, 1, 0, SETMAG, Di)
MAC_SAME_NAME_ARG(SetMagIndex, 1, 0, SETMAGINDEX, I)
MAC_SAME_NAME_ARG(ChangeMag, 1, 1, CHANGEMAG, I)
MAC_SAME_FUNC_ARG(IncMagIfFoundPixel, 1, 0, ChangeMag, INCMAGIFFOUNDPIXEL, I)
MAC_SAME_NAME_ARG(ChangeMagAndIntensity, 1, 1, CHANGEMAGANDINTENSITY, I)
MAC_SAME_FUNC_ARG(SetMagAndIntensity, 1, 0, ChangeMagAndIntensity, SETMAGANDINTENSITY, D)
MAC_SAME_NAME_ARG(SetCamLenIndex, 1, 0, SETCAMLENINDEX, I)
MAC_SAME_NAME_ARG(SetSpotSize, 1, 0, SETSPOTSIZE, I)
MAC_SAME_NAME_ARG(SetProbeMode, 1, 0, SETPROBEMODE, S)
MAC_SAME_NAME_ARG(Delay, 1, 1, DELAY, Ds)
MAC_SAME_NAME_ARG(WaitForMidnight, 0, 1, WAITFORMIDNIGHT, ddii)
MAC_SAME_NAME_ARG(WaitForDose, 1, 1, WAITFORDOSE, Di)
MAC_SAME_NAME_NOARG(ScreenUp, 0, 0, SCREENUP)
MAC_SAME_NAME_NOARG(ScreenDown, 0, 0, SCREENDOWN)
MAC_SAME_NAME_NOARG(ReportScreen, 0, 0, REPORTSCREEN)
MAC_SAME_NAME_NOARG(ReportScreenCurrent, 0, 0, REPORTSCREENCURRENT)
MAC_SAME_NAME_ARG(ImageShiftByPixels, 2, 1, IMAGESHIFTBYPIXELS, DDdi)
MAC_SAME_NAME_ARG(ImageShiftByUnits, 2, 1, IMAGESHIFTBYUNITS, DDdi)
MAC_SAME_NAME_ARG(ImageShiftByMicrons, 2, 1, IMAGESHIFTBYMICRONS, DDdi)
MAC_SAME_NAME_ARG(ImageShiftByStageDiff, 2, 0, IMAGESHIFTBYSTAGEDIFF, DDdi)
MAC_SAME_NAME_NOARG(ImageShiftToLastMultiHole, 0, 0, IMAGESHIFTTOLASTMULTIHOLE)
MAC_SAME_NAME_ARG(ShiftImageForDrift, 3, 0, SHIFTIMAGEFORDRIFT, DDI)
MAC_SAME_NAME_ARG(ShiftCalSkipLensNorm, 0, 4, SHIFTCALSKIPLENSNORM, i)
MAC_SAME_NAME_ARG(CalibrateImageShift, 0, 0, CALIBRATEIMAGESHIFT, i)
MAC_SAME_NAME_NOARG(ReportFocusDrift, 0, 0, REPORTFOCUSDRIFT)
MAC_SAME_NAME_ARG(TestSTEMshift, 3, 1, TESTSTEMSHIFT, DDD)
MAC_SAME_NAME_ARG(QuickFlyback, 2, 1, QUICKFLYBACK, SD)
MAC_SAME_NAME_NOARG(ReportAutoFocus, 0, 0, REPORTAUTOFOCUS)
MAC_SAME_NAME_NOARG(ReportTargetDefocus, 0, 0, REPORTTARGETDEFOCUS)
MAC_SAME_NAME_ARG(SetBeamShift, 2, 0, SETBEAMSHIFT, DD)
MAC_SAME_NAME_ARG(MoveBeamByMicrons, 2, 1, MOVEBEAMBYMICRONS, DD)
MAC_SAME_NAME_ARG(MoveBeamByFieldFraction, 2, 1, MOVEBEAMBYFIELDFRACTION, DD)
MAC_SAME_NAME_ARG(SetBeamTilt, 2, 0, SETBEAMTILT, DD)
MAC_SAME_NAME_NOARG(ReportBeamShift, 0, 0, REPORTBEAMSHIFT)
MAC_SAME_NAME_NOARG(ReportBeamTilt, 0, 0, REPORTBEAMTILT)
MAC_SAME_NAME_ARG(SetImageShift, 2, 0, SETIMAGESHIFT, DDdi)
MAC_SAME_NAME_NOARG(AdjustBeamTiltforIS, 0, 0, ADJUSTBEAMTILTFORIS)
MAC_SAME_NAME_NOARG(ReportImageShift, 0, 0, REPORTIMAGESHIFT)
MAC_SAME_NAME_ARG(SetObjectiveStigmator, 2, 0, SETOBJECTIVESTIGMATOR, DD)
MAC_SAME_NAME_ARG(ReportXLensDeflector, 1, 0, REPORTXLENSDEFLECTOR, I)
MAC_SAME_FUNC_ARG(SetXLensDeflector, 3, 0, ReportXLensDeflector, SETXLENSDEFLECTOR, IDD)
MAC_SAME_FUNC_NOARG(ReportXLensFocus, 0, 0, ReportXLensDeflector, REPORTXLENSFOCUS)
MAC_SAME_FUNC_ARG(SetXLensFocus, 1, 0, ReportXLensDeflector, SETXLENSFOCUS, D)
MAC_SAME_NAME_NOARG(ReportSpecimenShift, 0, 0, REPORTSPECIMENSHIFT)
MAC_SAME_NAME_NOARG(ReportObjectiveStigmator, 0, 0, REPORTOBJECTIVESTIGMATOR)
MAC_SAME_NAME_ARG(SuppressReports, 0, 4, SUPPRESSREPORTS, i)
MAC_SAME_NAME_ARG(ErrorsToLog, 0, 4, ERRORSTOLOG, i)
MAC_SAME_NAME_NOARG(ReportAlignShift, 0, 0, REPORTALIGNSHIFT)
MAC_SAME_FUNC_ARG(ReportShiftDiffFrom, 1, 0, ReportAlignShift, REPORTSHIFTDIFFFROM, D)
MAC_SAME_FUNC_NOARG(ReportISforBufferShift, 0, 0, ReportAlignShift, REPORTISFORBUFFERSHIFT)
MAC_SAME_NAME_NOARG(ReportAccumShift, 0, 0, REPORTACCUMSHIFT)
MAC_SAME_NAME_NOARG(ResetAccumShift, 0, 0, RESETACCUMSHIFT)
MAC_SAME_NAME_NOARG(ReportAlignTrimming, 0, 0, REPORTALIGNTRIMMING)
MAC_SAME_NAME_ARG(CameraToISMatrix, 1, 4, CAMERATOISMATRIX, I)
MAC_SAME_FUNC_ARG(ISToCameraMatrix, 1, 4, CameraToISMatrix, ISTOCAMERAMATRIX, I)
MAC_SAME_FUNC_ARG(CameraToStageMatrix, 1, 4, CameraToISMatrix, CAMERATOSTAGEMATRIX, I)
MAC_SAME_FUNC_ARG(StageToCameraMatrix, 1, 4, CameraToISMatrix, STAGETOCAMERAMATRIX, I)
MAC_SAME_FUNC_ARG(CameraToSpecimenMatrix, 1, 4, CameraToISMatrix, CAMERATOSPECIMENMATRIX, I)
MAC_SAME_FUNC_ARG(SpecimenToCameraMatrix, 1, 4, CameraToISMatrix, SPECIMENTOCAMERAMATRIX, I)
MAC_SAME_FUNC_ARG(ISToSpecimenMatrix, 1, 4, CameraToISMatrix, ISTOSPECIMENMATRIX, I)
MAC_SAME_FUNC_ARG(SpecimenToISMatrix, 1, 4, CameraToISMatrix, SPECIMENTOISMATRIX, I)
MAC_SAME_FUNC_ARG(ISToStageMatrix, 1, 4, CameraToISMatrix, ISTOSTAGEMATRIX, I)
MAC_SAME_FUNC_ARG(StageToISMatrix, 1, 4, CameraToISMatrix, STAGETOISMATRIX, I)
MAC_SAME_FUNC_ARG(StageToSpecimenMatrix, 1, 4, CameraToISMatrix, STAGETOSPECIMENMATRIX, I)
MAC_SAME_FUNC_ARG(SpecimenToStageMatrix, 1, 4, CameraToISMatrix, SPECIMENTOSTAGEMATRIX, I)
MAC_SAME_NAME_NOARG(ReportClock, 0, 0, REPORTCLOCK)
MAC_SAME_NAME_NOARG(ResetClock, 0, 4, RESETCLOCK)
MAC_SAME_NAME_NOARG(ReportMinuteTime, 0, 0, REPORTMINUTETIME)
MAC_SAME_NAME_ARG(SetCustomTime, 1, 4, SETCUSTOMTIME, Sd)
MAC_SAME_FUNC_ARG(ReportCustomInterval, 1, 0, SetCustomTime, REPORTCUSTOMINTERVAL, Sd)
MAC_SAME_NAME_NOARG(ReportTickTime, 0, 0, REPORTTICKTIME)
MAC_SAME_NAME_ARG(ElapsedTickTime, 1, 5, ELAPSEDTICKTIME, D)
MAC_SAME_NAME_NOARG(ReportDateTime, 0, 0, REPORTDATETIME)
MAC_SAME_NAME_ARG(MoveStage, 2, 1, MOVESTAGE, DDd)
MAC_SAME_FUNC_ARG(MoveStageTo, 2, 1, MoveStage, MOVESTAGETO, DDd)
MAC_SAME_FUNC_ARG(TestRelaxingStage, 2, 1, MoveStage, TESTRELAXINGSTAGE, DDdd)
MAC_SAME_FUNC_ARG(StageShiftByPixels, 2, 1, MoveStage, STAGESHIFTBYPIXELS, DD)
MAC_SAME_FUNC_NOARG(StageToLastMultiHole, 0, 0, MoveStage, STAGETOLASTMULTIHOLE)
MAC_SAME_FUNC_ARG(MoveStageWithSpeed, 3, 1, MoveStage, MOVESTAGEWITHSPEED, DDD)
MAC_SAME_NAME_ARG(RelaxStage, 0, 1, RELAXSTAGE, d)
MAC_SAME_NAME_ARG(BackgroundTilt, 1, 1, BACKGROUNDTILT, Dd)
MAC_SAME_NAME_NOARG(ReportStageXYZ, 0, 0, REPORTSTAGEXYZ)
MAC_SAME_NAME_NOARG(ReportTiltAngle, 0, 0, REPORTTILTANGLE)
MAC_SAME_NAME_NOARG(ReportStageBusy, 0, 0, REPORTSTAGEBUSY)
MAC_SAME_NAME_NOARG(ReportStageBAxis, 0, 0, REPORTSTAGEBAXIS)
MAC_SAME_NAME_NOARG(ReportMag, 0, 0, REPORTMAG)
MAC_SAME_NAME_NOARG(ReportMagIndex, 0, 0, REPORTMAGINDEX)
MAC_SAME_NAME_NOARG(ReportCameraLength, 0, 0, REPORTCAMERALENGTH)
MAC_SAME_NAME_NOARG(ReportDefocus, 0, 0, REPORTDEFOCUS)
MAC_SAME_NAME_NOARG(ReportFocus, 0, 0, REPORTFOCUS)
MAC_SAME_FUNC_NOARG(ReportAbsoluteFocus, 0, 0, ReportFocus, REPORTABSOLUTEFOCUS)
MAC_SAME_NAME_NOARG(ReportPercentC2, 0, 0, REPORTPERCENTC2)
MAC_SAME_NAME_NOARG(ReportCrossoverPercentC2, 0, 0, REPORTCROSSOVERPERCENTC2)
MAC_SAME_NAME_NOARG(ReportIlluminatedArea, 0, 0, REPORTILLUMINATEDAREA)
MAC_SAME_NAME_NOARG(ReportImageDistanceOffset, 0, 0, REPORTIMAGEDISTANCEOFFSET)
MAC_SAME_NAME_NOARG(ReportAlpha, 0, 0, REPORTALPHA)
MAC_SAME_NAME_NOARG(ReportSpotSize, 0, 0, REPORTSPOTSIZE)
MAC_SAME_NAME_NOARG(ReportProbeMode, 0, 0, REPORTPROBEMODE)
MAC_SAME_NAME_NOARG(ReportEnergyFilter, 0, 0, REPORTENERGYFILTER)
MAC_SAME_NAME_NOARG(ReportColumnMode, 0, 0, REPORTCOLUMNMODE)
MAC_SAME_NAME_ARG(ReportLens, 1, 0, REPORTLENS, S)
MAC_SAME_NAME_ARG(ReportCoil, 1, 0, REPORTCOIL, S)
MAC_SAME_NAME_ARG(SetFreeLensControl, 2, 1, SETFREELENSCONTROL, II)
MAC_SAME_NAME_ARG(SetLensWithFLC, 2, 0, SETLENSWITHFLC, IDi)
MAC_SAME_NAME_ARG(ReportLensFLCStatus, 1, 0, REPORTLENSFLCSTATUS, I)
MAC_SAME_NAME_ARG(SetJeolSTEMflags, 2, 2, SETJEOLSTEMFLAGS, DD)
MAC_SAME_NAME_NOARG(SetCameraPLAOffset, 0, 0, SETCAMERAPLAOFFSET)
MAC_SAME_NAME_ARG(RemoveAperture, 1, 0, REMOVEAPERTURE, I)
MAC_SAME_FUNC_ARG(ReInsertAperture, 1, 0, RemoveAperture, REINSERTAPERTURE, I)
MAC_SAME_NAME_NOARG(PhasePlateToNextPos, 0, 0, PHASEPLATETONEXTPOS)
MAC_SAME_NAME_NOARG(ReportPhasePlatePos, 0, 0, REPORTPHASEPLATEPOS)
MAC_SAME_NAME_ARG(ReportMeanCounts, 0, 0, REPORTMEANCOUNTS, si)
MAC_SAME_NAME_NOARG(ReportFileZsize, 0, 0, REPORTFILEZSIZE)
MAC_SAME_NAME_ARG(SubareaMean, 4, 5, SUBAREAMEAN, IIII)
MAC_SAME_NAME_ARG(CircularSubareaMean, 3, 4 , CIRCULARSUBAREAMEAN, DDD)
MAC_SAME_NAME_ARG(ElectronStats, 0, 4, ELECTRONSTATS, s)
MAC_SAME_FUNC_ARG(RawElectronStats, 0, 4, ElectronStats, RAWELECTRONSTATS, s)
MAC_SAME_NAME_ARG(CropImage, 5, 1, CROPIMAGE, SIIII)
MAC_SAME_FUNC_ARG(CropCenterToSize, 3, 0, CropImage, CROPCENTERTOSIZE, SII)
MAC_SAME_NAME_ARG(ReduceImage, 2, 1, REDUCEIMAGE, SD)
MAC_SAME_NAME_ARG(Rotate90CW, 1, 0, ROTATE90CW, S)
MAC_SAME_NAME_ARG(Rotate90CCW, 1, 0, ROTATE90CCW, S)
MAC_SAME_NAME_ARG(FFT, 1, 0, FFT, Si)
MAC_SAME_NAME_ARG(FilterImage, 5, 0, FILTERIMAGE, SDDDDs)
MAC_DIFF_NAME_ARG(AddImages, 2, 1, CombineImages, ADDIMAGES, SSs)
MAC_SAME_FUNC_ARG(SubtractImages, 2, 1, CombineImages, SUBTRACTIMAGES, SSs)
MAC_SAME_FUNC_ARG(MultiplyImages, 2, 1, CombineImages, MULTIPLYIMAGES, SSs)
MAC_SAME_FUNC_ARG(DivideImages, 2, 1, CombineImages, DIVIDEIMAGES, SSs)
MAC_SAME_NAME_ARG(ScaleImage, 3, 1, SCALEIMAGE, SDDsi)
MAC_SAME_NAME_ARG(CtfFind, 3, 1, CTFFIND, SDDiiddii)
MAC_SAME_NAME_ARG(ImageProperties, 0, 4, IMAGEPROPERTIES, s)
MAC_SAME_NAME_ARG(ImageLowDoseSet, 0, 4, IMAGELOWDOSESET, s)
MAC_SAME_NAME_ARG(MeasureBeamSize, 0, 4, MEASUREBEAMSIZE, s)
MAC_SAME_NAME_ARG(QuadrantMeans, 0, 4, QUADRANTMEANS, idd)
MAC_SAME_NAME_ARG(CheckForBadStripe, 0, 4, CHECKFORBADSTRIPE, si)
MAC_SAME_NAME_ARG(CircleFromPoints, 6, 1, CIRCLEFROMPOINTS, DDDDDD)
MAC_SAME_NAME_NOARG(FindPixelSize, 0, 0, FINDPIXELSIZE)
MAC_SAME_NAME_ARG(Echo, 0, 4, ECHO, s)
MAC_SAME_FUNC(EchoNoVarSub, 0, 4, Echo, ECHONOVARSUB)
MAC_SAME_NAME(EchoEval, 0, 5, ECHOEVAL)
MAC_SAME_FUNC_ARG(EchoReplaceLine, 0, 5, EchoEval, ECHOREPLACELINE, s)
MAC_SAME_FUNC_ARG(EchoNoLineEnd, 1, 5, EchoEval, ECHONOLINEEND, S)
MAC_SAME_NAME_ARG(Verbose, 1, 4, VERBOSE, D)
MAC_SAME_NAME_ARG(ProgramTimeStamps, 0, 4, PROGRAMTIMESTAMPS, i)
MAC_SAME_NAME_ARG(IsVersionAtLeast, 1, 4, ISVERSIONATLEAST, Ss)
MAC_SAME_FUNC_ARG(SkipIfVersionLessThan, 1, 4, IsVersionAtLeast, SKIPIFVERSIONLESSTHAN, Ss)
MAC_SAME_NAME_NOARG(IsFFTWindowOpen, 0, 0, ISFFTWINDOWOPEN)
MAC_SAME_NAME_ARG(ReportEnvironVar, 1, 0, REPORTENVIRONVAR, S)
MAC_SAME_NAME_NOARG(ReportSettingsFile, 0, 0, REPORTSETTINGSFILE)
MAC_SAME_NAME_NOARG(ListAllCalibrations, 0, 0, LISTALLCALIBRATIONS)
MAC_SAME_NAME_ARG(Pause, 0, 4, PAUSE, s)
MAC_SAME_FUNC_ARG(YesNoBox, 1, 4, Pause, YESNOBOX, S)
MAC_SAME_FUNC(AbortIfFailed, 1, 4, Pause, ABORTIFFAILED)
MAC_SAME_FUNC(PauseIfFailed, 0, 4, Pause, PAUSEIFFAILED)
MAC_SAME_NAME_ARG(OKBox, 1, 4, OKBOX, S)
MAC_SAME_NAME_ARG(EnterOneNumber, 1, 4, ENTERONENUMBER, S)
MAC_SAME_FUNC_ARG(EnterDefaultedNumber, 3, 4, EnterOneNumber, ENTERDEFAULTEDNUMBER, DIS)
MAC_SAME_NAME_ARG(EnterString, 2, 4, ENTERSTRING, SS)
MAC_SAME_NAME_ARG(ThreeChoiceBox, 4, 4, THREECHOICEBOX, SSSSs)
MAC_SAME_NAME_ARG(CompareNoCase, 2, 4, COMPARENOCASE, SS)
MAC_SAME_FUNC_ARG(CompareStrings, 2, 4, CompareNoCase, COMPARESTRINGS, SS)
MAC_SAME_NAME_ARG(StripEndingDigits, 2, 4, STRIPENDINGDIGITS, SS)
MAC_SAME_NAME_ARG(MailSubject, 1, 4, MAILSUBJECT, S)
MAC_SAME_NAME_ARG(SendEmail, 1, 0, SENDEMAIL, S)
MAC_SAME_FUNC_ARG(ErrorBoxSendEmail, 1, 4, SendEmail, ERRORBOXSENDEMAIL, S)
MAC_SAME_NAME_ARG(ClearAlignment, 0, 0, CLEARALIGNMENT, i)
MAC_SAME_NAME_ARG(ResetImageShift, 0, 0, RESETIMAGESHIFT, id)
MAC_SAME_NAME_ARG(ResetShiftIfAbove, 1, 1, RESETSHIFTIFABOVE, D)
MAC_SAME_NAME_ARG(Eucentricity, 0, 0, EUCENTRICITY, i)
MAC_SAME_NAME_NOARG(ReportLastAxisOffset, 0, 0, REPORTLASTAXISOFFSET)
MAC_SAME_NAME_ARG(SetTiltAxisOffset, 1, 4, SETTILTAXISOFFSET, D)
MAC_SAME_NAME_ARG(WalkUpTo, 1, 1, WALKUPTO, D)
MAC_SAME_NAME_ARG(ReverseTilt, 0, 0, REVERSETILT, i)
MAC_SAME_NAME_ARG(DriftWaitTask, 0, 1, DRIFTWAITTASK, dsddisiii)
MAC_SAME_NAME_ARG(ConditionPhasePlate, 0, 0, CONDITIONPHASEPLATE, i)
MAC_SAME_NAME_NOARG(GetWaitTaskDrift, 0, 0, GETWAITTASKDRIFT)
MAC_SAME_NAME_NOARG(BacklashAdjust, 0, 0, BACKLASHADJUST)
MAC_SAME_NAME_ARG(CenterBeamFromImage, 0, 0, CENTERBEAMFROMIMAGE, id)
MAC_SAME_NAME_ARG(AutoCenterBeam, 0, 0, AUTOCENTERBEAM, d)
MAC_SAME_NAME_NOARG(CookSpecimen, 0, 0, COOKSPECIMEN)
MAC_SAME_NAME_NOARG(SetIntensityByLastTilt, 0, 2, SETINTENSITYBYLASTTILT)
MAC_SAME_FUNC_ARG(ChangeIntensityBy, 1, 1, SetIntensityByLastTilt, CHANGEINTENSITYBY, D)
MAC_SAME_FUNC_ARG(SetIntensityForMean, 1, 0, SetIntensityByLastTilt, SETINTENSITYFORMEAN, D)
MAC_SAME_NAME_ARG(SetDoseRate, 1, 0, SETDOSERATE, D)
MAC_SAME_NAME_ARG(SetPercentC2, 1, 0, SETPERCENTC2, D)
MAC_SAME_FUNC_ARG(IncPercentC2, 1, 1, SetPercentC2, INCPERCENTC2, D)
MAC_SAME_NAME_ARG(SetIlluminatedArea, 1, 0, SETILLUMINATEDAREA, D)
MAC_SAME_NAME_ARG(SetImageDistanceOffset, 1, 0, SETIMAGEDISTANCEOFFSET, D)
MAC_SAME_NAME_ARG(SetAlpha, 1, 0, SETALPHA, I)
MAC_SAME_NAME_NOARG(ReportJeolGIF, 0, 0, REPORTJEOLGIF)
MAC_SAME_NAME_ARG(SetJeolGIF, 1, 0, SETJEOLGIF, I)
MAC_SAME_NAME_ARG(NormalizeLenses, 1, 0, NORMALIZELENSES, I)
MAC_SAME_NAME_ARG(NormalizeAllLenses, 0, 0, NORMALIZEALLLENSES, i)
MAC_SAME_NAME_ARG(ReportSlotStatus, 1, 4, REPORTSLOTSTATUS, I)
MAC_SAME_NAME_ARG(LoadCartridge, 1, 0, LOADCARTRIDGE, I)
MAC_SAME_FUNC_NOARG(UnloadCartridge, 0, 0, LoadCartridge, UNLOADCARTRIDGE)
MAC_SAME_NAME_ARG(RefrigerantLevel, 1, 4, REFRIGERANTLEVEL, I)
MAC_SAME_NAME_NOARG(DewarsRemainingTime, 0, 4, DEWARSREMAININGTIME)
MAC_SAME_NAME_NOARG(AreDewarsFilling, 0, 4, AREDEWARSFILLING)
MAC_SAME_NAME_ARG(ReportVacuumGauge, 1, 0, REPORTVACUUMGAUGE, S)
MAC_SAME_NAME_NOARG(ReportHighVoltage, 0, 0, REPORTHIGHVOLTAGE)
MAC_SAME_NAME_ARG(SetSlitWidth, 1, 0, SETSLITWIDTH, D)
MAC_SAME_NAME_ARG(SetEnergyLoss, 1, 0, SETENERGYLOSS, D)
MAC_SAME_FUNC_ARG(ChangeEnergyLoss, 1, 1, SetEnergyLoss, CHANGEENERGYLOSS, D)
MAC_SAME_NAME_ARG(SetSlitIn, 0, 1, SETSLITIN, i)
MAC_SAME_NAME_ARG(RefineZLP, 0, 0, REFINEZLP, d)
MAC_SAME_NAME_ARG(SelectCamera, 1, 0, SELECTCAMERA, I)
MAC_SAME_NAME_ARG(SetExposure, 2, 0, SETEXPOSURE, SDd)
MAC_SAME_NAME_ARG(SetBinning, 2, 4, SETBINNING, SI)
MAC_SAME_NAME_ARG(SetCameraArea, 2, 4, SETCAMERAAREA, SSiii)
MAC_SAME_NAME_ARG(SetCenteredSize, 4, 4, SETCENTEREDSIZE, SIII)
MAC_SAME_NAME_ARG(SetExposureForMean, 1, 0, SETEXPOSUREFORMEAN, Di)
MAC_SAME_NAME_ARG(SetContinuous, 2, 2, SETCONTINUOUS, SI)
MAC_SAME_NAME_ARG(SetProcessing, 2, 4, SETPROCESSING, SI)
MAC_SAME_NAME_ARG(SetFrameTime, 2, 4, SETFRAMETIME, SD)
MAC_SAME_FUNC_ARG(ChangeFrameAndExposure, 2, 1, SetFrameTime, CHANGEFRAMEANDEXPOSURE, SD)
MAC_SAME_NAME_ARG(SetK2ReadMode, 2, 4, SETK2READMODE, SI)
MAC_SAME_NAME_ARG(SetDoseFracParams, 2, 4, SETDOSEFRACPARAMS, SIiiii)
MAC_SAME_NAME_ARG(SetDECamFrameRate, 1, 4, SETDECAMFRAMERATE, D)
MAC_SAME_NAME_ARG(UseContinuousFrames, 1, 4, USECONTINUOUSFRAMES, I)
MAC_SAME_NAME_NOARG(StopContinuous, 0, 0, STOPCONTINUOUS)
MAC_SAME_NAME_NOARG(ReportContinuous, 0, 4, REPORTCONTINUOUS)
MAC_SAME_NAME_ARG(StartFrameWaitTimer, 0, 4, STARTFRAMEWAITTIMER, i)
MAC_SAME_NAME_ARG(WaitForNextFrame, 0, 0, WAITFORNEXTFRAME, ii)
MAC_SAME_NAME_ARG(SetLiveSettleFraction, 1, 4, SETLIVESETTLEFRACTION, D)
MAC_SAME_NAME_ARG(SetSTEMDetectors, 2, 0, SETSTEMDETECTORS, SIiiiiiii)
MAC_DIFF_NAME_ARG(RestoreCameraSet, 0, 4, RestoreCameraSetCmd, RESTORECAMERASET, s)
MAC_SAME_NAME_ARG(KeepCameraSetChanges, 0, 4, KEEPCAMERASETCHANGES, s)
MAC_SAME_NAME_NOARG(ReportK2FileParams, 0, 0, REPORTK2FILEPARAMS)
MAC_SAME_NAME_ARG(SetK2FileParams, 1, 4, SETK2FILEPARAMS, Iiiiii)
MAC_SAME_NAME_NOARG(ReportFrameAliParams, 0, 0, REPORTFRAMEALIPARAMS)
MAC_SAME_FUNC_ARG(SetFrameAliParams, 1, 4, ReportFrameAliParams, SETFRAMEALIPARAMS, Iiiiii)
MAC_SAME_FUNC_ARG(SetFrameAli2, 1, 4, ReportFrameAliParams, SETFRAMEALI2, Ididdd)
MAC_SAME_FUNC_NOARG(ReportFrameAli2, 0, 0, ReportFrameAliParams, REPORTFRAMEALI2)
MAC_SAME_NAME_ARG(SetFolderForFrames, 1, 4, SETFOLDERFORFRAMES, S)
MAC_SAME_NAME_ARG(SkipFrameAliParamCheck, 0, 4, SKIPFRAMEALIPARAMCHECK, i)
MAC_SAME_NAME_NOARG(ReportK3CDSmode, 0, 0, REPORTK3CDSMODE)
MAC_SAME_NAME_ARG(SetK3CDSmode, 1, 4, SETK3CDSMODE, Ii)
MAC_SAME_NAME_NOARG(ReportCountScaling, 0, 0, REPORTCOUNTSCALING)
MAC_SAME_NAME_ARG(SetDivideBy2, 1, 4, SETDIVIDEBY2, I)
MAC_SAME_NAME_NOARG(ReportNumFramesSaved, 0, 0, REPORTNUMFRAMESSAVED)
MAC_SAME_NAME_ARG(CameraProperties, 0, 4, CAMERAPROPERTIES, ii)
MAC_SAME_NAME_NOARG(ReportColumnOrGunValve, 0, 0, REPORTCOLUMNORGUNVALVE)
MAC_SAME_NAME_ARG(SetColumnOrGunValve, 1, 0, SETCOLUMNORGUNVALVE, I)
MAC_SAME_NAME_NOARG(ReportFilamentCurrent, 0, 0, REPORTFILAMENTCURRENT)
MAC_SAME_NAME_ARG(SetFilamentCurrent, 1, 0, SETFILAMENTCURRENT, D)
MAC_SAME_NAME_NOARG(IsPVPRunning, 0, 4, ISPVPRUNNING)
MAC_SAME_NAME_ARG(SetBeamBlank, 1, 0, SETBEAMBLANK, I)
MAC_SAME_NAME_ARG(MoveToNavItem, 0, 0, MOVETONAVITEM, i)
MAC_SAME_NAME_ARG(RealignToNavItem, 1, 0, REALIGNTONAVITEM, Iidii)
MAC_SAME_FUNC_ARG(RealignToOtherItem, 2, 0, RealignToNavItem, REALIGNTOOTHERITEM, IIidid)
MAC_SAME_NAME_ARG(RealignToMapDrawnOn, 2, 0, REALIGNTOMAPDRAWNON, II)
MAC_SAME_NAME_NOARG(GetRealignToItemError, 0, 4, GETREALIGNTOITEMERROR)
MAC_SAME_NAME_NOARG(ReportNavItem, 0, 0, REPORTNAVITEM)
MAC_SAME_FUNC_ARG(ReportOtherItem, 1, 0, ReportNavItem, REPORTOTHERITEM, I)
MAC_SAME_FUNC_ARG(LoadNavMap, 0, 0, ReportNavItem, LOADNAVMAP, s)
MAC_SAME_FUNC_ARG(LoadOtherMap, 1, 0, ReportNavItem, LOADOTHERMAP, Is)
MAC_SAME_FUNC_NOARG(ReportNextNavAcqItem, 0, 4, ReportNavItem, REPORTNEXTNAVACQITEM)
MAC_SAME_NAME_ARG(ReportItemAcquire, 0, 0, REPORTITEMACQUIRE, i)
MAC_SAME_NAME_ARG(SetItemAcquire, 0, 0, SETITEMACQUIRE, ii)
MAC_SAME_NAME_ARG(NavIndexWithLabel, 1, 4, NAVINDEXWITHLABEL, S)
MAC_SAME_FUNC_ARG(NavIndexWithNote, 1, 4, NavIndexWithLabel, NAVINDEXWITHNOTE, S)
MAC_SAME_NAME_ARG(NavIndexItemDrawnOn, 1, 4, NAVINDEXITEMDRAWNON, I)
MAC_SAME_NAME_ARG(NavItemFileToOpen, 1, 4, NAVITEMFILETOOPEN, I)
MAC_SAME_NAME_ARG(SetNavItemUserValue, 2, 6, SETNAVITEMUSERVALUE, IIs)
MAC_SAME_FUNC_ARG(ReportItemUserValue, 2, 0, SetNavItemUserValue, REPORTITEMUSERVALUE, II)
MAC_SAME_NAME_ARG(SetMapAcquireState, 1, 0, SETMAPACQUIRESTATE, I)
MAC_SAME_NAME_NOARG(RestoreState, 0, 0, RESTORESTATE)
MAC_SAME_NAME_NOARG(ReportNumNavAcquire, 0, 0, REPORTNUMNAVACQUIRE)
MAC_SAME_NAME_ARG(ReportNumHoleAcquire, 0, 0, REPORTNUMHOLEACQUIRE, iii)
MAC_SAME_NAME_NOARG(ReportNumTableItems, 0, 4, REPORTNUMTABLEITEMS)
MAC_SAME_NAME_ARG(SetNavRegistration, 1, 4, SETNAVREGISTRATION, I)
MAC_SAME_NAME_ARG(SaveNavigator, 0, 4, SAVENAVIGATOR, s)
MAC_SAME_NAME_NOARG(ReportIfNavOpen, 0, 4, REPORTIFNAVOPEN)
MAC_SAME_NAME_ARG(ReadNavFile, 1, 4, READNAVFILE, S)
MAC_SAME_FUNC_ARG(MergeNavFile, 1, 4, ReadNavFile, MERGENAVFILE, S)
MAC_SAME_NAME_NOARG(CloseNavigator, 0, 0, CLOSENAVIGATOR)
MAC_SAME_NAME_ARG(OpenNavigator, 0, 0, OPENNAVIGATOR, s)
MAC_SAME_NAME_ARG(ChangeItemRegistration, 2, 5, CHANGEITEMREGISTRATION, II)
MAC_SAME_FUNC_ARG(ChangeItemColor, 2, 4, ChangeItemRegistration, CHANGEITEMCOLOR, II)
MAC_SAME_FUNC_ARG(ChangeItemLabel, 2, 4, ChangeItemRegistration, CHANGEITEMLABEL, IS)
MAC_SAME_FUNC_ARG(ChangeItemNote, 1, 4, ChangeItemRegistration, CHANGEITEMNOTE, Is)
MAC_SAME_NAME_ARG(SetItemTargetDefocus, 2, 4, SETITEMTARGETDEFOCUS, ID)
MAC_SAME_NAME_ARG(SetItemSeriesAngles, 3, 4, SETITEMSERIESANGLES, IDDd)
MAC_SAME_NAME_NOARG(SkipAcquiringNavItem, 0, 4, SKIPACQUIRINGNAVITEM)
MAC_SAME_NAME_ARG(SkipAcquiringGroup, 0, 4, SKIPACQUIRINGGROUP, i)
MAC_SAME_NAME_ARG(SkipMoveInNavAcquire, 0, 4, SKIPMOVEINNAVACQUIRE, i)
MAC_SAME_NAME_ARG(SuspendNavRedraw, 0, 4, SUSPENDNAVREDRAW, i)
MAC_SAME_NAME_NOARG(StartNavAcquireAtEnd, 0, 0, STARTNAVACQUIREATEND)
MAC_SAME_NAME_ARG(SuffixForExtraFile, 1, 4, SUFFIXFOREXTRAFILE, Sssssss)
MAC_SAME_NAME_ARG(ItemForSuperCoord, 1, 4, ITEMFORSUPERCOORD, I)
MAC_SAME_NAME_NOARG(UpdateItemZ, 0, 4, UPDATEITEMZ)
MAC_SAME_FUNC_ARG(UpdateGroupZ, 0, 4, UpdateItemZ, UPDATEGROUPZ, i)
MAC_SAME_NAME_NOARG(ReportGroupStatus, 0, 0, REPORTGROUPSTATUS)
MAC_SAME_NAME_ARG(ReportItemImageCoords, 0, 4, REPORTITEMIMAGECOORDS, is)
MAC_SAME_NAME_ARG(NewMap, 0, 0, NEWMAP, is)
MAC_SAME_NAME_NOARG(MakeAnchorMap, 0, 0, MAKEANCHORMAP)
MAC_SAME_NAME_NOARG(ShiftItemsByAlignment, 0, 4, SHIFTITEMSBYALIGNMENT)
MAC_SAME_NAME_ARG(ShiftItemsByCurrentDiff, 1, 4, SHIFTITEMSBYCURRENTDIFF, D)
MAC_SAME_NAME_ARG(ShiftItemsByMicrons, 2, 5, SHIFTITEMSBYMICRONS, DDi)
MAC_SAME_NAME_ARG(AlignAndTransformItems, 2, 0, ALIGNANDTRANSFORMITEMS, DDd)
MAC_SAME_NAME_NOARG(ForceCenterRealign, 0, 4, FORCECENTERREALIGN)
MAC_SAME_NAME_ARG(SkipPiecesOutsideItem, 1, 4, SKIPPIECESOUTSIDEITEM, I)
MAC_SAME_NAME_ARG(FindHoles, 0, 0, FINDHOLES, s)
MAC_SAME_NAME_ARG(MakeNavPointsAtHoles, 0, 4, MAKENAVPOINTSATHOLES, idddd)
MAC_SAME_NAME_NOARG(ClearHoleFinder, 0, 4, CLEARHOLEFINDER)
MAC_SAME_NAME_ARG(CombineHolesToMulti, 1, 4, COMBINEHOLESTOMULTI, I)
MAC_SAME_NAME_NOARG(UndoHoleCombining, 0, 4, UNDOHOLECOMBINING)
MAC_SAME_NAME_ARG(SetHelperParams, 1, 4, SETHELPERPARAMS, Dddd)
MAC_SAME_NAME_ARG(SetMontageParams, 1, 4, SETMONTAGEPARAMS, Iiiiii)
MAC_SAME_NAME_ARG(ManualFilmExposure, 1, 1, MANUALFILMEXPOSURE, D)
MAC_SAME_NAME_NOARG(ExposeFilm, 0, 1, EXPOSEFILM)
MAC_SAME_FUNC_ARG(SpecialExposeFilm, 1, 1, ExposeFilm, SPECIALEXPOSEFILM, Ddi)
MAC_SAME_NAME_ARG(GoToLowDoseArea, 1, 0, GOTOLOWDOSEAREA, S)
MAC_SAME_NAME_ARG(SetLDContinuousUpdate, 1, 4, SETLDCONTINUOUSUPDATE, I)
MAC_SAME_NAME_ARG(SetLowDoseMode, 1, 0, SETLOWDOSEMODE, I)
MAC_SAME_NAME_ARG(SetAxisPosition, 2, 0, SETAXISPOSITION, SDd)
MAC_SAME_FUNC_ARG(ReportAxisPosition, 1, 0, SetAxisPosition, REPORTAXISPOSITION, S)
MAC_SAME_NAME_NOARG(ReportLowDose, 0, 4, REPORTLOWDOSE)
MAC_SAME_NAME_ARG(CurrentSettingsToLDArea, 1, 0, CURRENTSETTINGSTOLDAREA, S)
MAC_SAME_NAME_ARG(UpdateLowDoseParams, 1, 0, UPDATELOWDOSEPARAMS, Si)
MAC_DIFF_NAME_ARG(RestoreLowDoseParams, 0, 0, RestoreLowDoseParamsCmd, RESTORELOWDOSEPARAMS, s)
MAC_SAME_NAME_ARG(SetLDAddedBeamButton, 0, 4, SETLDADDEDBEAMBUTTON, i)
MAC_SAME_NAME_ARG(ShowMessageOnScope, 1, 0, SHOWMESSAGEONSCOPE, S)
MAC_SAME_NAME_ARG(SetupScopeMessage, 1, 6, SETUPSCOPEMESSAGE, Ids)
MAC_SAME_NAME_ARG(UpdateHWDarkRef, 1, 1, UPDATEHWDARKREF, D)
MAC_SAME_NAME_ARG(LongOperation, 1, 1, LONGOPERATION, Sssssssssssssssssss)
MAC_SAME_NAME_ARG(NewDEserverDarkRef, 2, 0, NEWDESERVERDARKREF, ID)
MAC_SAME_NAME_ARG(RunInShell, 1, 0, RUNINSHELL, S)
MAC_SAME_NAME_ARG(NextProcessArgs, 1, 4, NEXTPROCESSARGS, Sssssssssssssssssss)
MAC_SAME_NAME_ARG(CreateProcess, 1, 0, CREATEPROCESS, S)
MAC_SAME_NAME_ARG(ExternalToolArgPlace, 1, 0, EXTERNALTOOLARGPLACE, I)
MAC_SAME_NAME_ARG(RunExternalTool, 1, 0, RUNEXTERNALTOOL, S)
MAC_SAME_NAME_NOARG(SaveFocus, 0, 4, SAVEFOCUS)
MAC_SAME_NAME_NOARG(RestoreFocus, 0, 4, RESTOREFOCUS)
MAC_SAME_NAME_NOARG(SaveBeamTilt, 0, 4, SAVEBEAMTILT)
MAC_SAME_NAME_NOARG(RestoreBeamTilt, 0, 0, RESTOREBEAMTILT)
MAC_SAME_NAME_ARG(SelectPiezo, 2, 0, SELECTPIEZO, II)
MAC_SAME_NAME_NOARG(ReportPiezoXY, 0, 0, REPORTPIEZOXY)
MAC_SAME_NAME_NOARG(ReportPiezoZ, 0, 0, REPORTPIEZOZ)
MAC_SAME_NAME_ARG(MovePiezoXY, 2, 1, MOVEPIEZOXY, DDi)
MAC_SAME_NAME_ARG(MovePiezoZ, 1, 1, MOVEPIEZOZ, Di)
MAC_SAME_FUNC_ARG(SetFloatVariable, 2, 0, SetVariableCmd, SETFLOATVARIABLE, SDi)
MAC_SAME_NAME(PythonScript, 0, 0, PYTHONSCRIPT)
MAC_SAME_FUNC(EndPythonScript, 1, 4, NoOperation, ENDPYTHONSCRIPT)
MAC_SAME_NAME(FindScriptByName, 1, 4, FINDSCRIPTBYNAME)
MAC_SAME_NAME_NOARG(ReportFEGEmissionState, 0, 4, REPORTFEGEMISSIONSTATE)
MAC_SAME_NAME_ARG(SetFEGEmissionState, 1, 4, SETFEGEMISSIONSTATE, I)
MAC_SAME_NAME_ARG(SetImageBeamTilt, 2, 0, SETIMAGEBEAMTILT, DD)
MAC_SAME_NAME_NOARG(ReportImageBeamTilt, 0, 0, REPORTIMAGEBEAMTILT)
MAC_SAME_NAME_NOARG(ListCalibrations, 0, 4, LISTCALIBRATIONS)
MAC_SAME_NAME_NOARG(ReportCurrentBuffer, 0, 0, REPORTCURRENTBUFFER)
MAC_SAME_FUNC_ARG(ChangeItemDraw, 1, 4, ChangeItemRegistration, CHANGEITEMDRAW, Ii)
MAC_SAME_NAME_ARG(IsFEGFlashingAdvised, 1, 4, ISFEGFLASHINGADVISED, I)
MAC_SAME_NAME_ARG(NextFEGFlashHighTemp, 0, 4, NEXTFEGFLASHHIGHTEMP, i)
MAC_SAME_NAME_NOARG(SimpleOriginStatus, 0, 4, SIMPLEORIGINSTATUS)
MAC_SAME_NAME_ARG(AlignToTemplate, 0, 0, ALIGNTOTEMPLATE, ddiis)
MAC_SAME_NAME_ARG(ManageDewarsAndPumps, 0, 0, MANAGEDEWARSANDPUMPS, i)

// new Python-only commands need to be added to pythonOnlyCmds in ::CMacroProcessor
//
// The longest command name is now 25 characters but 23 is a more common limit
// # of args, 1 for arith allowed + 2 for not allowed in Set... + 4 looping in OnIdle OK

//Undefine ALL the macros to ready it for the next use
#undef MAC_DIFF_NAME
#undef MAC_DIFF_NAME_ARG
#undef MAC_DIFF_NAME_NOARG
#undef MAC_SAME_FUNC
#undef MAC_SAME_FUNC_ARG
#undef MAC_SAME_FUNC_NOARG
#undef MAC_SAME_NAME
#undef MAC_SAME_NAME_ARG
#undef MAC_SAME_NAME_NOARG
