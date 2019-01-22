'List of installed OLEDB providers on local computer
Option Explicit
Const HKEY_CLASSES_ROOT     = &H80000000
Const HKEY_CURRENT_USER     = &H80000001
Const HKEY_LOCAL_MACHINE    = &H80000002
Const HKEY_USERS        = &H80000003
Const HKEY_CURRENT_CONFIG   = &H80000005

Dim OutText, strComputer, objRegistry
Dim num
Dim ProgIdDict

strComputer = "."
Set objRegistry = GetObject("winmgmts:\\" & strComputer & "\root\default:StdRegProv")
OutText = "Note: Strike Ctrl+C to copy full text to clipboard"
Num = 1
Set ProgIdDict = CreateObject("Scripting.Dictionary")

' I discovered these registrations can appear in three different places.
' Use ProgIdDict to prevent dupes in the output
Append objRegistry, HKEY_CLASSES_ROOT, "HKEY_CLASSES_ROOT", "CLSID", ProgIdDict, Num, OutText
Append objRegistry, HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE", "SOFTWARE\Classes\CLSID", ProgIdDict, Num, OutText
Append objRegistry, HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE", "SOFTWARE\Classes\Wow6432Node\CLSID", ProgIdDict, Num, OutText

Sub Append(ByVal objRegistry, ByVal HKEYConstant, ByVal HKEYConstantStr, ByVal KeyPrefixStr, ByVal ProgIdDict, ByRef Num, ByRef OutText)

    Dim Key, arrKeys
    Dim strKeyPath, strValue, uValue

    objRegistry.enumKey HKEYConstant, KeyPrefixStr, arrKeys

    for each key in arrKeys

        strKeyPath = KeyPrefixStr & "\" & key

        ' if key exists...
        ' I noticed something weird where non-MSOLAP entries use the first style,
        ' and MSOLAP entries use the second style.
        If 0 = objRegistry.GetDWordValue(HKEYConstant, strKeyPath, "OLEDB_SERVICES", uValue) _
        Or 0 = objRegistry.GetDWordValue(HKEYConstant, strKeyPath & "\OLEDB_SERVICES", "", uValue) _
        Then
            objRegistry.GetStringValue HKEYConstant,strKeyPath & "\ProgID","",strValue
            If Not ProgIdDict.Exists(strValue) _
            Then
                ProgIdDict.Add strValue, strValue
                OutText=OutText & vbcrlf & vbcrlf
                'get the (Default) value which is the name of the provider
                objRegistry.GetStringValue HKEYConstant,strKeyPath,"",strValue
                OutText=OutText & num & ") " & strValue & vbcrlf & "Key: \\" & HKEYConstantStr & "\" & KeyPrefixStr & "\" & key
                ' and the expanded description
                objRegistry.GetStringValue HKEYConstant,strKeyPath & "\OLE DB Provider","",strValue
                OutText=OutText & vbcrlf & "OLE DB Provider: " & strValue
                objRegistry.GetStringValue HKEYConstant,strKeyPath & "\ProgID","",strValue
                OutText=OutText & vbcrlf & "ProgID: " & strValue
                objRegistry.GetStringValue HKEYConstant,strKeyPath & "\VersionIndependentProgID","",strValue
                OutText=OutText & vbcrlf & "VersionIndependentProgID: " & strValue
                num = 1 + num
            End If
        end if
    next

End Sub

Wscript.Echo OutText