BOOL GetTextualSid(
    PSID pSid,            // binary SID
    LPTSTR TextualSid,    // buffer for Textual representation of SID
    LPDWORD lpdwBufferLen // required/provided TextualSid buffersize
    )
{
    PSID_IDENTIFIER_AUTHORITY psia;
    DWORD dwSubAuthorities;
    DWORD dwSidRev=SID_REVISION;
    DWORD dwCounter;
    DWORD dwSidSize;
 
    // Validate the binary SID.
 
    if(!IsValidSid(pSid)) return FALSE;
 
    // Get the identifier authority value from the SID.
 
    psia = GetSidIdentifierAuthority(pSid);
 
    // Get the number of subauthorities in the SID.
 
    dwSubAuthorities = *GetSidSubAuthorityCount(pSid);
 
    // Compute the buffer length.
    // S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL
 
    dwSidSize=(15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);
 
    // Check input buffer length.
    // If too small, indicate the proper size and set the last error.
 
    if (*lpdwBufferLen <dwSidSize)
    {
        *lpdwBufferLen = dwSidSize;
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        return FALSE;
    }
 
    // Add 'S' prefix and revision number to the string.
 
    dwSidSize=wsprintf(TextualSid, TEXT("S-%lu-"), dwSidRev );
 
    // Add a SID identifier authority to the string.
 
    if ( (psia->Value[0] != 0) || (psia->Value[1] != 0) )
    {
        dwSidSize+=wsprintf(TextualSid + lstrlen(TextualSid),
                    TEXT("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
                    (USHORT)psia->Value[0],
                    (USHORT)psia->Value[1],
                    (USHORT)psia->Value[2],
                    (USHORT)psia->Value[3],
                    (USHORT)psia->Value[4],
                    (USHORT)psia->Value[5]);
    }
    else
    {
        dwSidSize+=wsprintf(TextualSid + lstrlen(TextualSid),
                    TEXT("%lu"),
                    (ULONG)(psia->Value[5]      )   +
                    (ULONG)(psia->Value[4] << 8)   +
                    (ULONG)(psia->Value[3] <<16)   +
                    (ULONG)(psia->Value[2] <<24)   );
    }
 
    // Add SID subauthorities to the string.
    //
    for (dwCounter=0 ; dwCounter <dwSubAuthorities ; dwCounter++)
    {
        dwSidSize+=wsprintf(TextualSid + dwSidSize, TEXT("-%lu"),
                    *GetSidSubAuthority(pSid, dwCounter) );
    }
 
    return TRUE;
}
 
PSID pSidGetOwnerSid(LPSTR lpUser)
{
    DWORD        dwSidLen = 0, dwDomainLen = 0;
    SID_NAME_USE    SidNameUse;
 
    //The function on the first call retives the length that we need
    //to initialize the SID & domain name pointers
    if(!LookupAccountName(NULL, lpUser, NULL, &dwSidLen, 
                   NULL, &dwDomainLen, &SidNameUse))
    {
        if(ERROR_INSUFFICIENT_BUFFER == GetLastError())
        {
            PSID    pSid = LocalAlloc(LMEM_ZEROINIT, dwSidLen);
            LPSTR    lpDomainName = (LPSTR)LocalAlloc(LMEM_ZEROINIT, dwDomainLen);
 
            if(pSid && lpDomainName &&
                LookupAccountName(NULL, lpUser, pSid, &dwSidLen,
                lpDomainName, &dwDomainLen, &SidNameUse))
                return pSid;
        }
    }
 
    printf("\nImpossible de récuperer le SID - %d", GetLastError());
 
    return NULL;    //Was not able to retrive PSID
}
