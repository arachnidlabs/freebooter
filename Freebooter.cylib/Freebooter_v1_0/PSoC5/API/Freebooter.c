/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
*  Description:
*   Provides an API for the Bootloader component. The API includes functions
*   for starting boot loading operations, validating the application and
*   jumping to the application.
*
********************************************************************************
* Portions of this code are copyright (c) 2013 Nick Johnson.
* Portions of this code are copyright (c) 2013 Cypress Semiconductor Corporation.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
* 
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "`$INSTANCE_NAME`.h"
#include "device.h"
#include <string.h>


/*******************************************************************************
* Defines
*******************************************************************************/

#define `$INSTANCE_NAME`_VERSION        {`$CY_MINOR_VERSION`, `$CY_MAJOR_VERSION`, 0x01u}

/* Packet framing constants. */
#define `$INSTANCE_NAME`_SOP            (0x01u)    /* Start of Packet */
#define `$INSTANCE_NAME`_EOP            (0x17u)    /* End of Packet */


/* Bootloader command responces */
#define `$INSTANCE_NAME`_ERR_KEY       (0x01u)  /* The provided key does not match the expected value          */
#define `$INSTANCE_NAME`_ERR_VERIFY    (0x02u)  /* The verification of flash failed                            */
#define `$INSTANCE_NAME`_ERR_LENGTH    (0x03u)  /* The amount of data available is outside the expected range  */
#define `$INSTANCE_NAME`_ERR_DATA      (0x04u)  /* The data is not of the proper form                          */
#define `$INSTANCE_NAME`_ERR_CMD       (0x05u)  /* The command is not recognized                               */
#define `$INSTANCE_NAME`_ERR_DEVICE    (0x06u)  /* The expected device does not match the detected device      */
#define `$INSTANCE_NAME`_ERR_VERSION   (0x07u)  /* The bootloader version detected is not supported            */
#define `$INSTANCE_NAME`_ERR_CHECKSUM  (0x08u)  /* The checksum does not match the expected value              */
#define `$INSTANCE_NAME`_ERR_ARRAY     (0x09u)  /* The flash array is not valid                                */
#define `$INSTANCE_NAME`_ERR_ROW       (0x0Au)  /* The flash row is not valid                                  */
#define `$INSTANCE_NAME`_ERR_PROTECT   (0x0Bu)  /* The flash row is protected and can not be programmed        */
#define `$INSTANCE_NAME`_ERR_APP       (0x0Cu)  /* The application is not valid and cannot be set as active    */
#define `$INSTANCE_NAME`_ERR_ACTIVE    (0x0Du)  /* The application is currently marked as active               */
#define `$INSTANCE_NAME`_ERR_UNK       (0x0Fu)  /* An unknown error occurred                                   */


/* Bootloader command definitions. */
#define `$INSTANCE_NAME`_COMMAND_CHECKSUM     (0x31u)    /* Verify the checksum for the bootloadable project   */
#define `$INSTANCE_NAME`_COMMAND_REPORT_SIZE  (0x32u)    /* Report the programmable portions of flash          */
#define `$INSTANCE_NAME`_COMMAND_APP_STATUS   (0x33u)    /* Gets status info about the provided app status     */
#define `$INSTANCE_NAME`_COMMAND_ERASE        (0x34u)    /* Erase the specified flash row                      */
#define `$INSTANCE_NAME`_COMMAND_SYNC         (0x35u)    /* Sync the bootloader and host application           */
#define `$INSTANCE_NAME`_COMMAND_APP_ACTIVE   (0x36u)    /* Sets the active application                        */
#define `$INSTANCE_NAME`_COMMAND_DATA         (0x37u)    /* Queue up a block of data for programming           */
#define `$INSTANCE_NAME`_COMMAND_ENTER        (0x38u)    /* Enter the bootloader                               */
#define `$INSTANCE_NAME`_COMMAND_PROGRAM      (0x39u)    /* Program the specified row                          */
#define `$INSTANCE_NAME`_COMMAND_VERIFY       (0x3Au)    /* Compute flash row checksum for verification        */
#define `$INSTANCE_NAME`_COMMAND_EXIT         (0x3Bu)    /* Exits the bootloader & resets the chip             */


/*******************************************************************************
* Bootloader packet byte addresses:
* [1-byte] [1-byte ] [2-byte] [n-byte] [ 2-byte ] [1-byte]
* [ SOP  ] [Command] [ Size ] [ Data ] [Checksum] [ EOP  ]
*******************************************************************************/
#define `$INSTANCE_NAME`_SOP_ADDR             (0x00u)         /* Start of packet offset from beginning     */
#define `$INSTANCE_NAME`_CMD_ADDR             (0x01u)         /* Command offset from beginning             */
#define `$INSTANCE_NAME`_SIZE_ADDR            (0x02u)         /* Packet size offset from beginning         */
#define `$INSTANCE_NAME`_DATA_ADDR            (0x04u)         /* Packet data offset from beginning         */
#define `$INSTANCE_NAME`_CHK_ADDR(x)          (0x04u + (x)) /* Packet checksum offset from end           */
#define `$INSTANCE_NAME`_EOP_ADDR(x)          (0x06u + (x)) /* End of packet offset from end             */
#define `$INSTANCE_NAME`_MIN_PKT_SIZE         (7u)            /* The minimum number of bytes in a packet   */


#if(1u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)

    /* None is active by default */
    uint8 `$INSTANCE_NAME`_activeApp = 2u;

#endif  /* (1u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */


/* Our definition of a row size. */
#if((!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0))

    #define `$INSTANCE_NAME`_FROW_SIZE          ((CYDEV_FLS_ROW_SIZE) + (CYDEV_ECC_ROW_SIZE))

#else

    #define `$INSTANCE_NAME`_FROW_SIZE          CYDEV_FLS_ROW_SIZE

#endif  /* ((!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0)) */


#if(CY_PSOC4)

    #define `$INSTANCE_NAME`_BL_LAST_ROW            (CY_GET_XTND_REG16(`$INSTANCE_NAME`_MD_LAST_BLDR_ROW_ADDR  ))
    #define `$INSTANCE_NAME`_BL_APP_RUN_ADDR        (CY_GET_XTND_REG8 (`$INSTANCE_NAME`_MD_APP_RUN_ADDR        ))
    #define `$INSTANCE_NAME`_BL_APP_ENTRY_ADDR      (CY_GET_XTND_REG32(`$INSTANCE_NAME`_MD_APP_ENTRY_POINT_ADDR))
    #define `$INSTANCE_NAME`_BL_APP_BYTE_LEN        (CY_GET_XTND_REG32(`$INSTANCE_NAME`_MD_APP_BYTE_LEN        ))
    #define `$INSTANCE_NAME`_BL_APP_VERIFIED        (CY_GET_XTND_REG8 (`$INSTANCE_NAME`_MD_APP_VERIFIED_ADDR   ))
    #define `$INSTANCE_NAME`_BL_APP_CHECKSUM        (CY_GET_XTND_REG8 (`$INSTANCE_NAME`_MD_CHECKSUM_ADDR       ))

    #define `$INSTANCE_NAME`_INIT_FLASH_WRITE       (1)
    #define `$INSTANCE_NAME`_FLASH_NUMBER_SECTORS   (1)
    #define `$INSTANCE_NAME`_CHECKSUM_START         (0xC0u) /* Exclude the vector table as these get remapped to SRAM */

#else

    #define `$INSTANCE_NAME`_BL_LAST_ROW            (*`$INSTANCE_NAME`_MD_PTR_LAST_BLDR_ROW    )
    #define `$INSTANCE_NAME`_BL_APP_RUN_ADDR        (*`$INSTANCE_NAME`_MD_PTR_APP_RUN_ADDR     )
    #define `$INSTANCE_NAME`_BL_APP_ENTRY_ADDR      (*`$INSTANCE_NAME`_MD_PTR_APP_ENTRY_POINT  )
    #define `$INSTANCE_NAME`_BL_APP_BYTE_LEN        (*`$INSTANCE_NAME`_MD_PTR_APP_BYTE_LEN     )
    #define `$INSTANCE_NAME`_BL_APP_VERIFIED        (*`$INSTANCE_NAME`_MD_PTR_APP_VERIFIED     )
    #define `$INSTANCE_NAME`_BL_APP_CHECKSUM        (*`$INSTANCE_NAME`_MD_PTR_CHECKSUM         )

    #define `$INSTANCE_NAME`_INIT_FLASH_WRITE       ((CYRET_SUCCESS == CySetTemp()) && \
                                                     (CYRET_SUCCESS == CySetFlashEEBuffer(flashBuffer)))

    #define `$INSTANCE_NAME`_FLASH_NUMBER_SECTORS   (CY_FLASH_NUMBER_ARRAYS)

    #define `$INSTANCE_NAME`_CHECKSUM_START         0    /* The bootloader always starts at 0 in flash */

    uint8 flashBuffer[`$INSTANCE_NAME`_FROW_SIZE];

#endif  /* (CY_PSOC4) */


#define `$INSTANCE_NAME`_FIRST_APP_BYTE             (CYDEV_FLS_ROW_SIZE * (`$INSTANCE_NAME`_BL_LAST_ROW + 1))
#define `$INSTANCE_NAME`_SIZEOF_COMMAND_BUFFER      (300u) /* Maximum number of bytes accepted in a packet plus some */


typedef struct _`$INSTANCE_NAME`_ENTER
{
    uint32 SiliconId;
    uint8  Revision;
    uint8  BootLoaderVersion[3u];

} `$INSTANCE_NAME`_ENTER;


/*******************************************************************************
* The Checksum and SizeBytes are forcefully set in code. We then post process
* the hex file from the linker and inject their values then. When the hex file
* is loaded onto the device these two variables should have valid values.
* Because the compiler can do optimizations remove the constant
* accesses, these should not be accessed directly. Instead, the variables
* CyBtldr_ChecksumAccess & CyBtldr_SizeBytesAccess should be used to get the
* proper values at runtime.
*******************************************************************************/
#if (defined(__C51__))
    const uint8  CYCODE `$INSTANCE_NAME`_Checksum = 0;
    const uint32 CYCODE `$INSTANCE_NAME`_SizeBytes = 0xFFFFFFFF;
#else
    const uint8  CYCODE __attribute__((section (".bootloader"))) `$INSTANCE_NAME`_Checksum = 0;
    const uint32 CYCODE __attribute__((section (".bootloader"))) `$INSTANCE_NAME`_SizeBytes = 0xFFFFFFFF;
#endif  /* (defined(__C51__)) */

uint8  *`$INSTANCE_NAME`_ChecksumAccess =  (uint8* )(&`$INSTANCE_NAME`_Checksum);
uint32 *`$INSTANCE_NAME`_SizeBytesAccess = (uint32*)(&`$INSTANCE_NAME`_SizeBytes);


/***************************************
*     Function Prototypes
***************************************/
int    `$INSTANCE_NAME`_WritePacket(uint8 command, uint8 CYXDATA* buffer, uint16 size) CYSMALL `=ReentrantKeil("`$INSTANCE_NAME`_WritePacket")`;
uint16 `$INSTANCE_NAME`_CalcPacketChecksum(uint8* buffer, uint16 size) CYSMALL `=ReentrantKeil("`$INSTANCE_NAME`_CalcPacketChecksum")`;
uint8   `$INSTANCE_NAME`_Calc8BitFlashSum(uint32 start, uint32 size) CYSMALL `=ReentrantKeil("`$INSTANCE_NAME`_Calc8BitFlashSum")`;


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_CalcPacketChecksum
********************************************************************************
*
* Summary:
*  This computes the 16 bit checksum for the provided number of bytes contained
*  in the provided buffer
*
* Parameters:
*  buffer:
*     The buffer containing the data to compute the checksum for
*  size:
*     The number of bytes in buffer to compute the checksum for
*
* Returns:
*  16 bit checksum for the provided data
*
*******************************************************************************/
uint16 `$INSTANCE_NAME`_CalcPacketChecksum(uint8* buffer, uint16 size) CYSMALL `=ReentrantKeil("`$INSTANCE_NAME`_CalcPacketChecksum")`
{
    #if(`$INSTANCE_NAME`_PACKET_CHECKSUM_CRC == 1u)

        uint16 CYDATA crc = 0xffff;
        uint16 CYDATA tmp;
        uint8  CYDATA i;

        if(size == 0)
        {
            return(~crc);
        }

        do
        {
            for (i = 0, tmp = 0x00ff & *buffer++; i < 8; i++, tmp >>= 1)
            {
                if ((crc & 0x0001) ^ (tmp & 0x0001))
                {
                    crc = (crc >> 1) ^ 0x8408;
                }
                else
                {
                    crc >>= 1;
                }
            }
        }
        while(--size);

        crc = ~crc;
        tmp = crc;
        crc = (crc << 8) | (tmp >> 8 & 0xFF);

        return(crc);

    #else

        uint16 CYDATA sum = 0;
        while (size-- > 0)
        {
            sum += *buffer++;
        }

        return(1 + ~sum);

    #endif /* (`$INSTANCE_NAME`_PACKET_CHECKSUM_CRC == 1u) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Calc8BitFlashSum
********************************************************************************
*
* Summary:
*  This computes the 8 bit sum for the provided number of bytes contained in
*  flash.
*
* Parameters:
*  start:
*     The starting address to start summing data for
*  size:
*     The number of bytes to read and compute the sum for
*
* Returns:
*   8 bit sum for the provided data
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_Calc8BitFlashSum(uint32 start, uint32 size) CYSMALL `=ReentrantKeil("`$INSTANCE_NAME`_Calc8BitFlashSum")`
{
    uint8 CYDATA sum = 0;

    while (size-- > 0)
    {
        sum += `$INSTANCE_NAME`_GET_CODE_DATA(start + size);
    }

    return(sum);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
* Summary:
*  Runs the bootloading algorithm, determining if a bootload is necessary and
*  launching the application if it is not.
*
* Parameters:
*  None
*
* Return:
*  This method will never return. It will either load a new application and
*  reset the device or it will jump directly to the existing application.
*
* Remark:
*  If this method determines that the bootloader appliation itself is corrupt,
*  this method will not return, instead it will simply hang the application.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Start(void) CYSMALL `=ReentrantKeil("`$INSTANCE_NAME`_Start")`
{
    uint8 CYDATA calcedChecksum;

    /* Identify active bootloadable application */
    #if(0u != `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)

        uint8 i;
        for (i = 0; i < CYDEV_BOOTLOADER_APPLICATIONS; i++)
        {
            if (*`$INSTANCE_NAME`_P_APP_ACTIVE(i) == 1u)
            {
                `$INSTANCE_NAME`_activeApp = i;
                break;
            }
        }

    #endif  /* (0u != `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */


    /* Bootloader Application Validation */
    #if(0u != `$INSTANCE_NAME`_BOOTLOADABLE_APP_VALID)

        calcedChecksum = `$INSTANCE_NAME`_Calc8BitFlashSum(`$INSTANCE_NAME`_CHECKSUM_START,
                *`$INSTANCE_NAME`_SizeBytesAccess - `$INSTANCE_NAME`_CHECKSUM_START);

        /* we actually included the checksum, so remove it */
        calcedChecksum -= *`$INSTANCE_NAME`_ChecksumAccess;
        calcedChecksum = 1 + ~calcedChecksum;

        /* Halt device if bootloader application is invalid */
        if((calcedChecksum != *`$INSTANCE_NAME`_ChecksumAccess) ||
            !(*`$INSTANCE_NAME`_SizeBytesAccess) || !(`$INSTANCE_NAME`_INIT_FLASH_WRITE))
        {
            CyHalt(0x00u);
        }

    #endif  /* (0u != `$INSTANCE_NAME`_BOOTLOADABLE_APP_VALID) */


    if (`$INSTANCE_NAME`_ValidateApp(`$INSTANCE_NAME`_activeApp) ||
    #if defined (WORKAROUND_OPT_XRES)
        (`$INSTANCE_NAME`_`$INSTANCE_NAME`_BL_APP_RUN_ADDR) == `$INSTANCE_NAME`_START_BTLDR)
    #else
        (`$INSTANCE_NAME`_GET_RUN_TYPE == `$INSTANCE_NAME`_START_BTLDR))
    #endif  /* defined (WORKAROUND_OPT_XRES) */
        {
            `$INSTANCE_NAME`_SET_RUN_TYPE(0);

            `$INSTANCE_NAME`_HostLink(0);
        }


    #if(1u == `$INSTANCE_NAME`_WAIT_FOR_COMMAND)

        /* Timeout is in 10s of miliseconds */
        `$INSTANCE_NAME`_HostLink(`$INSTANCE_NAME`_WAIT_FOR_COMMAND_TIME);

    #endif  /* (1u == `$INSTANCE_NAME`_WAIT_FOR_COMMAND) */


    `$INSTANCE_NAME`_LaunchApplication();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_LaunchApplication
********************************************************************************
*
* Summary:
*  Jumps the PC to the start address of the user application in flash.
*
* Parameters:
*  None
*
* Returns:
*  This method will never return if it succesfully goes to the user application.
*
*******************************************************************************/
void `$INSTANCE_NAME`_LaunchApplication(void) CYSMALL `=ReentrantKeil("`$INSTANCE_NAME`_LaunchApplication")`
{
    `$INSTANCE_NAME`_SET_RUN_TYPE(`$INSTANCE_NAME`_START_APP);

    /* Perform software reset */
    `$INSTANCE_NAME`_SOFTWARE_RESET;
}


/*******************************************************************************
* Function Name: CyBtldr_CheckLaunch
********************************************************************************
*
* Summary:
*  This routine checks to see if the bootloader or the bootloadable application
*  should be run.  If the application is to be run, it will start executing.
*  If the bootloader is to be run, it will return so the bootloader can
*  continue starting up.
*
* Parameters:
*  None
*
* Returns:
*  None
*
*******************************************************************************/
void CyBtldr_CheckLaunch(void) CYSMALL `=ReentrantKeil("CyBtldr_CheckLaunch")`
{
    #if defined (WORKAROUND_OPT_XRES)

        if((`$INSTANCE_NAME`_BL_APP_RUN_ADDR) == `$INSTANCE_NAME`_START_APP)
        {
            if(CYRET_SUCCESS != CySetTemp() || CYRET_SUCCESS != CySetFlashEEBuffer(flashBuffer))
            {
                CyHalt(0x00u);
            }

    #else

        if (`$INSTANCE_NAME`_GET_RUN_TYPE == `$INSTANCE_NAME`_START_APP)
        {

    #endif  /* defined (WORKAROUND_OPT_XRES) */

            `$INSTANCE_NAME`_SET_RUN_TYPE(0);

            /*******************************************************************************
            * Indicates that we have told ourselves to jump to the application since we have
            * already told ourselves to jump, we do not do any expensive verification of the
            * application. We just check to make sure that the value at CY_APP_ADDR_ADDRESS
            * is something other than 0.
            *******************************************************************************/
            if(`$INSTANCE_NAME`_BL_APP_ENTRY_ADDR)
            {
                /* Never return from this method */
                LaunchApp(`$INSTANCE_NAME`_BL_APP_ENTRY_ADDR);
            }
        }
}


/* Moves the arguement appAddr (RO) into PC, moving execution to the appAddr */
#if defined (__ARMCC_VERSION)
__asm void LaunchApp(uint32 appAddr)
{
#if(CY_PSOC5)
    EXTERN CyResetStatus
    LDR R2, =CyResetStatus
    LDR R2, [R2]
    NOP
#endif  /* (CY_PSOC5) */
    BX  R0

    ALIGN
}
#elif defined(__GNUC__)
__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void LaunchApp(uint32 appAddr)
{
#if(CY_PSOC5)
    __asm volatile(
        "    MOV R2, %0\n"
        "    LDR  R2, [R2]\n"
        "    BX  %1\n"
        : : "r" (CyResetStatus), "r" (appAddr)
        : "r0", "r2");
#else
    __asm volatile("    BX  R0\n");
#endif  /* (CY_PSOC5) */
}
#endif  /* (__ARMCC_VERSION) */


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ValidateApplication
********************************************************************************
* Summary:
*  This routine computes the checksum, zero check, 0xFF check of the
*  application area to determine whether a valid application is loaded.
*
* Parameters:
*  appId:
*      The application number to verify
*
* Returns:
*  CYRET_SUCCESS  - if successful
*  CYRET_BAD_DATA - if the bootloadable is corrupt
*
*******************************************************************************/
#if(0u != `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)
    cystatus `$INSTANCE_NAME`_ValidateApp(uint8 appId) CYSMALL `=ReentrantKeil("`$INSTANCE_NAME`_ValidateApp")`
    {
#else
    cystatus `$INSTANCE_NAME`_ValidateApplication() CYSMALL `=ReentrantKeil("`$INSTANCE_NAME`_ValidateApplication")`
    {
#endif  /* (0u != `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */
        uint32 CYDATA idx;
        uint32 CYDATA end = `$INSTANCE_NAME`_FIRST_APP_BYTE + `$INSTANCE_NAME`_BL_APP_BYTE_LEN;
        CYBIT         valid = 0; /* Assume bad flash image */
        uint8  CYDATA calcedChecksum = 0;


        #if(0u != `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)

            if(appId > 1u)
            {
                return(CYRET_BAD_DATA);
            }

        #endif  /* (0u != `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */


        #if(1u == `$INSTANCE_NAME`_FAST_APP_VALIDATION)

            if(`$INSTANCE_NAME`_BL_APP_VERIFIED)
            {
                return(CYRET_SUCCESS);
            }

        #endif  /* (1u == `$INSTANCE_NAME`_FAST_APP_VALIDATION) */


        /* Calculate checksum of bootloadable image */
        for(idx = `$INSTANCE_NAME`_FIRST_APP_BYTE; idx < end; ++idx)
        {
            uint8 CYDATA curByte = `$INSTANCE_NAME`_GET_CODE_DATA(idx);

            if((curByte != 0) && (curByte != 0xFF))
            {
                valid = 1;
            }

            calcedChecksum += curByte;
        }


        /***************************************************************************
        * We do not compute checksum over the meta data section, so no need to
        * subtract off App Verified or App Active information here like we do when
        * verifying a row.
        ***************************************************************************/


        #if((!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0))

            /* Add ECC data to checksum */
            idx = ((`$INSTANCE_NAME`_FIRST_APP_BYTE) >> 3);

            /* Flash may run into meta data, ECC does not so use full row */
            end = (end == (FLASH_SIZE - `$INSTANCE_NAME`_META_DATA_SIZE))
                ? FLASH_SIZE >> 3
                : end >> 3;

            for (; idx < end; ++idx)
            {
                calcedChecksum += CY_GET_XTND_REG8((void CYFAR *)(CYDEV_ECC_BASE + idx));
            }

        #endif  /* ((!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0)) */


        calcedChecksum = 1 + ~calcedChecksum;
        if((calcedChecksum != `$INSTANCE_NAME`_BL_APP_CHECKSUM) || !valid)
        {
            return(CYRET_BAD_DATA);
        }


        #if(1u == `$INSTANCE_NAME`_FAST_APP_VALIDATION)
            `$INSTANCE_NAME`_SetFlashByte(`$INSTANCE_NAME`_MD_APP_VERIFIED_ADDR, 1);
        #endif  /* (1u == `$INSTANCE_NAME`_FAST_APP_VALIDATION) */


        return(CYRET_SUCCESS);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_HostLink
********************************************************************************
*
* Summary:
*  Causes the bootloader to attempt to read data being transmitted by the
*  host application.  If data is sent from the host, this establishes the
*  communication interface to process all requests.
*
* Parameters:
*  timeOut:
*   The amount of time to listen for data before giving up. Timeout is
*   measured in 10s of ms.  Use 0 for infinite wait.
*
* Return:
*   None
*
*******************************************************************************/
void `$INSTANCE_NAME`_HostLink(uint8 timeOut) `=ReentrantKeil("`$INSTANCE_NAME`_HostLink")`
{
    uint16    CYDATA numberRead;
    uint16    CYDATA rspSize;
    uint8     CYDATA ackCode;
    uint16    CYDATA pktChecksum;
    cystatus  CYDATA readStat;
    uint16    CYDATA pktSize = 0;
    uint16 CYDATA dataOffset = 0;

    #if(0u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)
        uint8 CYDATA clearedMetaData = 0;
    #endif  /* (0u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */

    CYBIT     communicationActive = (timeOut == 0);
    uint8     packetBuffer[`$INSTANCE_NAME`_SIZEOF_COMMAND_BUFFER];
    uint8     dataBuffer  [`$INSTANCE_NAME`_SIZEOF_COMMAND_BUFFER];


    if(timeOut == 0u)
    {
        timeOut = 0xFF;
    }

    /* Initialize communications channel. */
    CyBtldrCommStart();

    /* Enable global interrupts */
    CyGlobalIntEnable;

    do
    {
        ackCode = CYRET_SUCCESS;
        readStat = CyBtldrCommRead(packetBuffer, `$INSTANCE_NAME`_SIZEOF_COMMAND_BUFFER, &numberRead, timeOut);

        if(readStat == CYRET_TIMEOUT || readStat == CYRET_EMPTY)
        {
            continue;
        }

        if((numberRead < `$INSTANCE_NAME`_MIN_PKT_SIZE) ||
           (packetBuffer[`$INSTANCE_NAME`_SOP_ADDR] != `$INSTANCE_NAME`_SOP))
        {
            ackCode = `$INSTANCE_NAME`_ERR_DATA;
        }
        else
        {
            pktSize = CY_GET_REG16(&packetBuffer[`$INSTANCE_NAME`_SIZE_ADDR]);
            pktChecksum = CY_GET_REG16(&packetBuffer[`$INSTANCE_NAME`_CHK_ADDR(pktSize)]);

            if((pktSize + `$INSTANCE_NAME`_MIN_PKT_SIZE) > numberRead)
            {
                ackCode = `$INSTANCE_NAME`_ERR_LENGTH;
            }
            else if(packetBuffer[`$INSTANCE_NAME`_EOP_ADDR(pktSize)] != `$INSTANCE_NAME`_EOP)
            {
                ackCode = `$INSTANCE_NAME`_ERR_DATA;
            }
            else if(pktChecksum != `$INSTANCE_NAME`_CalcPacketChecksum(packetBuffer, pktSize + `$INSTANCE_NAME`_DATA_ADDR))
            {
                ackCode = `$INSTANCE_NAME`_ERR_CHECKSUM;
            }
            else
            {
                /* Empty section */
            }
        }

        rspSize = 0;
        if(ackCode == CYRET_SUCCESS)
        {
            uint8 CYDATA btldrData = packetBuffer[`$INSTANCE_NAME`_DATA_ADDR];

            ackCode = `$INSTANCE_NAME`_ERR_DATA;
            switch(packetBuffer[`$INSTANCE_NAME`_CMD_ADDR])
            {


            /***************************************************************************
            *   Verify Checksum
            ***************************************************************************/
            case `$INSTANCE_NAME`_COMMAND_CHECKSUM:

                if(communicationActive && (pktSize == 0u))
                {
                    rspSize = 1;
                    packetBuffer[`$INSTANCE_NAME`_DATA_ADDR] =
                                (`$INSTANCE_NAME`_ValidateApp(`$INSTANCE_NAME`_activeApp) == CYRET_SUCCESS);
                    ackCode = CYRET_SUCCESS;
                }
                break;


            /***************************************************************************
            *   Get Flash Size
            ***************************************************************************/
            #if(`$INSTANCE_NAME`_CMD_GET_FLASH_SIZE_AVAIL == 1u)

                case `$INSTANCE_NAME`_COMMAND_REPORT_SIZE:

                    if(communicationActive && (pktSize == 1u))
                    {
                        if(btldrData < `$INSTANCE_NAME`_FLASH_NUMBER_SECTORS)
                        {
                            uint16 CYDATA startRow = (btldrData == 0)
                                ? (*`$INSTANCE_NAME`_SizeBytesAccess / CYDEV_FLS_ROW_SIZE)
                                : 0;

                            packetBuffer[`$INSTANCE_NAME`_DATA_ADDR]     = LO8(startRow);
                            packetBuffer[`$INSTANCE_NAME`_DATA_ADDR + 1] = HI8(startRow);
                            packetBuffer[`$INSTANCE_NAME`_DATA_ADDR + 2] = LO8(FLASH_NUMBER_ROWS - 1);
                            packetBuffer[`$INSTANCE_NAME`_DATA_ADDR + 3] = HI8(FLASH_NUMBER_ROWS - 1);
                        }

                        rspSize = 4;
                        ackCode = CYRET_SUCCESS;
                    }
                    break;

            #endif  /* (`$INSTANCE_NAME`_CMD_GET_FLASH_SIZE_AVAIL == 1u) */


            /***************************************************************************
            *   Get Application Status
            ***************************************************************************/
            #if(1u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)

                #if(1u == `$INSTANCE_NAME`_CMD_GET_APP_STATUS_AVAIL)

                    case `$INSTANCE_NAME`_COMMAND_APP_STATUS:

                        if(communicationActive && (pktSize == 1u))
                        {
                            rspSize = 2u;
                            packetBuffer[`$INSTANCE_NAME`_DATA_ADDR] = `$INSTANCE_NAME`_ValidateApp(btldrData);

                            packetBuffer[`$INSTANCE_NAME`_DATA_ADDR + 1] =
                                        (uint8)(*`$INSTANCE_NAME`_P_APP_ACTIVE(btldrData));

                            ackCode = CYRET_SUCCESS;
                        }
                        break;

                #endif  /* (1u == `$INSTANCE_NAME`_CMD_GET_APP_STATUS_AVAIL) */

            #endif  /* (1u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */


            /***************************************************************************
            *   Erase Row
            ***************************************************************************/
            #if(`$INSTANCE_NAME`_CMD_ERASE_ROW_AVAIL == 1u)

                case `$INSTANCE_NAME`_COMMAND_ERASE:

                    if(communicationActive && (pktSize != 3u))
                    {
                        break;
                    }

                    dataOffset = `$INSTANCE_NAME`_FROW_SIZE;
                    memset(dataBuffer, 0u, `$INSTANCE_NAME`_FROW_SIZE);

                    /* Intentional fall through to write the row */

            #endif  /* (`$INSTANCE_NAME`_CMD_ERASE_ROW_AVAIL == 1u) */


            /***************************************************************************
            *   Program Row
            ***************************************************************************/
            case `$INSTANCE_NAME`_COMMAND_PROGRAM:

                if(communicationActive && (pktSize >= 3u))
                {
                    memcpy(&dataBuffer[dataOffset], &packetBuffer[`$INSTANCE_NAME`_DATA_ADDR + 3], pktSize - 3);
                    dataOffset += (pktSize - 3);

                    pktSize = `$INSTANCE_NAME`_FROW_SIZE;

                    if(dataOffset == pktSize)
                    {
                        dataOffset = CY_GET_REG16(&(packetBuffer[`$INSTANCE_NAME`_DATA_ADDR + 1]));

                        #if(0u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)

                            if(!clearedMetaData)
                            {
                                uint8 erase[`$INSTANCE_NAME`_FROW_SIZE];

                                memset(erase, 0, `$INSTANCE_NAME`_FROW_SIZE);

                                #if(CY_PSOC4)
                                    CySysFlashWriteRow(`$INSTANCE_NAME`_MD_ROW, erase);
                                #else
                                    CyWriteRowFull(`$INSTANCE_NAME`_META_ARRAY, `$INSTANCE_NAME`_MD_ROW, erase, `$INSTANCE_NAME`_FROW_SIZE);
                                #endif  /* (CY_PSOC4) */

                                clearedMetaData = 1;
                            }

                        #else

                            if(`$INSTANCE_NAME`_activeApp < 2u)
                            {
                                #if(CY_PSOC4)

                                    uint16 row = dataOffset;
                                    uint16 firstRow = CY_GET_REG32(\
                                        `$INSTANCE_NAME`_META_LAST_BLDR_ROW_ADDR(`$INSTANCE_NAME`_activeApp)) + 1;

                                #else

                                    uint16 row = (btldrData * (CYDEV_FLS_SECTOR_SIZE / CYDEV_FLS_ROW_SIZE)) +
                                                  dataOffset;

                                    uint16 firstRow = (*((uint16 CYCODE *)\
                                        (`$INSTANCE_NAME`_META_LAST_BLDR_ROW_ADDR(`$INSTANCE_NAME`_activeApp)))) +
                                        1;

                                #endif  /* (CY_PSOC4) */

                                /***********************************************************************************
                                * Last row is equal to the first row plus the number of rows available for each app.
                                * To compute this, we first subtract the number of appliaction images from the total
                                * flash rows: (FLASH_NUMBER_ROWS - 2u).
                                *
                                * Then subtract off the first row:
                                * App Rows = (FLASH_NUMBER_ROWS - 2u - firstRow)
                                * Then divide that number by the number of application that must fit within the
                                * space, if we are app1 then that number is 2, if app2 then 1.  Our divisor is then:
                                * (2u - `$INSTANCE_NAME`_activeApp).
                                *
                                * Adding this number to firstRow gives the address right beyond our valid range so
                                * we subtract 1.
                                ***********************************************************************************/
                                uint16 lastRow = (firstRow - 1) +
                                                 (((CYDEV_FLS_SIZE / CYDEV_FLS_ROW_SIZE) - 2u - firstRow) /
                                                  (2u - `$INSTANCE_NAME`_activeApp));


                                /***********************************************************************************
                                * Check to see if the row to program is within the range of the active application,
                                * or if it maches the active application's meta data row.  If so, refuse to program
                                * as it would corrupt the active app.
                                ***********************************************************************************/
                                if((row >= firstRow && row <= lastRow) ||
                                   (btldrData == `$INSTANCE_NAME`_META_ARRAY &&
                                   dataOffset == `$INSTANCE_NAME`_MD_ROW(`$INSTANCE_NAME`_activeApp)))
                                {
                                    ackCode = `$INSTANCE_NAME`_ERR_ACTIVE;
                                    dataOffset = 0;
                                    break;
                                }
                            }

                        #endif  /* (0u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */


                        #if(CY_PSOC4)

                            ackCode = CySysFlashWriteRow(dataOffset, dataBuffer) \
                                ? `$INSTANCE_NAME`_ERR_ROW \
                                : CYRET_SUCCESS;

                        #else

                            ackCode = CyWriteRowFull(btldrData, dataOffset, dataBuffer, pktSize) \
                                ? `$INSTANCE_NAME`_ERR_ROW \
                                : CYRET_SUCCESS;

                        #endif  /* (CY_PSOC4) */

                    }
                    else
                    {
                        ackCode = `$INSTANCE_NAME`_ERR_LENGTH;
                    }

                    dataOffset = 0;
                }
                break;


            /***************************************************************************
            *   Sync Bootloader
            ***************************************************************************/
            #if(1u == `$INSTANCE_NAME`_CMD_SYNC_BOOTLOADER_AVAIL)

            case `$INSTANCE_NAME`_COMMAND_SYNC:

                /* If something failed the host would send this command to reset the bootloader. */
                dataOffset = 0;
                continue; /* Don't ack the packet, just get ready to accept the next one */

            #endif  /* (1u == `$INSTANCE_NAME`_CMD_SYNC_BOOTLOADER_AVAIL) */


            /***************************************************************************
            *   Set Active Application
            ***************************************************************************/
            #if(1u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)

                case `$INSTANCE_NAME`_COMMAND_APP_ACTIVE:

                    if(communicationActive && (pktSize == 1))
                    {
                        if(!`$INSTANCE_NAME`_ValidateApp(btldrData))
                        {
                            uint8 CYDATA idx;

                            for(idx = 0; idx < 2u; idx++)
                            {
                                `$INSTANCE_NAME`_SetFlashByte(`$INSTANCE_NAME`_META_APP_ACTIVE_ADDR(idx), idx == btldrData);
                            }
                            `$INSTANCE_NAME`_activeApp = btldrData;
                            ackCode = CYRET_SUCCESS;
                        }
                        else
                        {
                            ackCode = `$INSTANCE_NAME`_ERR_APP;
                        }
                    }
                    break;

            #endif  /* (1u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */


            /***************************************************************************
            *   Send Data
            ***************************************************************************/
            #if(`$INSTANCE_NAME`_CMD_SEND_DATA_AVAIL == 1u)

                case `$INSTANCE_NAME`_COMMAND_DATA:

                    /*  Make sure that dataOffset is valid before copying the data */
                    if((dataOffset + pktSize) <= `$INSTANCE_NAME`_SIZEOF_COMMAND_BUFFER)
                    {
                        ackCode = CYRET_SUCCESS;
                        memcpy(&dataBuffer[dataOffset], &packetBuffer[`$INSTANCE_NAME`_DATA_ADDR], pktSize);
                        dataOffset += pktSize;
                    }
                    else
                    {
                        ackCode = `$INSTANCE_NAME`_ERR_LENGTH;
                    }

                    break;

            #endif  /* (`$INSTANCE_NAME`_CMD_SEND_DATA_AVAIL == 1u) */


            /***************************************************************************
            *   Enter Bootloader
            ***************************************************************************/
            case `$INSTANCE_NAME`_COMMAND_ENTER:

                if(pktSize == 0)
                {
                    #if(CY_PSOC3)

                        `$INSTANCE_NAME`_ENTER CYDATA BtldrVersion =
                            {CYSWAP_ENDIAN32(CYDEV_CHIP_JTAG_ID), CYDEV_CHIP_REV_EXPECT, `$INSTANCE_NAME`_VERSION};

                    #else

                        `$INSTANCE_NAME`_ENTER CYDATA BtldrVersion =
                            {CYDEV_CHIP_JTAG_ID, CYDEV_CHIP_REV_EXPECT, `$INSTANCE_NAME`_VERSION};

                    #endif  /* (CY_PSOC3) */

                    communicationActive = 1;
                    rspSize = sizeof(`$INSTANCE_NAME`_ENTER);
                    memcpy(&packetBuffer[`$INSTANCE_NAME`_DATA_ADDR], &BtldrVersion, rspSize);
                    ackCode = CYRET_SUCCESS;
                }
                break;


            /***************************************************************************
            *   Verify Row
            ***************************************************************************/
            case `$INSTANCE_NAME`_COMMAND_VERIFY:

                if(communicationActive && (pktSize == 3u))
                {
                    /* Read the existing flash data. */
                    uint16 CYDATA rowNum = CY_GET_REG16(&(packetBuffer[`$INSTANCE_NAME`_DATA_ADDR + 1]));

                    uint32 CYDATA rowAddr = ((uint32)btldrData * CYDEV_FLS_SECTOR_SIZE)
                                            + ((uint32)rowNum * CYDEV_FLS_ROW_SIZE);

                    uint8 CYDATA checksum = `$INSTANCE_NAME`_Calc8BitFlashSum(rowAddr, CYDEV_FLS_ROW_SIZE);


                    #if(!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0)

                        /* Save the ECC area. */
                        uint16 CYDATA index;

                        rowAddr = CYDEV_ECC_BASE + ((uint32)btldrData * CYDEV_FLS_SECTOR_SIZE / 8)
                                    + ((uint32)rowNum * CYDEV_ECC_ROW_SIZE);

                        for(index = 0; index < CYDEV_ECC_ROW_SIZE; index++)
                        {
                            checksum += CY_GET_XTND_REG8((uint8 CYFAR *)(rowAddr + index));
                        }

                    #endif  /* (!CY_PSOC4) && (CYDEV_ECC_ENABLE == 0) */


                    /*******************************************************************************
                    * App Verified & App Active are information that is updated in flash at runtime
                    * remove these items from the checksum to allow the host to verify everything is
                    * correct.
                     ******************************************************************************/
                    #if(1u == `$INSTANCE_NAME`_DUAL_APP_BOOTLOADER)
                        if(`$INSTANCE_NAME`_META_ARRAY == btldrData && \
                            (`$INSTANCE_NAME`_MD_ROW(0) == rowNum || `$INSTANCE_NAME`_MD_ROW(1) == rowNum))
                    #else
                        if(`$INSTANCE_NAME`_META_ARRAY == btldrData && `$INSTANCE_NAME`_MD_ROW == rowNum)
                    #endif  /* (1u == $INSTANCE_NAME`_DUAL_APP_BOOTLOADER) */
                        {
                            uint8 appId = ((rowNum & 1) == 1) ? 0 : 1;

                            checksum -= *`$INSTANCE_NAME`_P_APP_ACTIVE(appId);
                            checksum -= *`$INSTANCE_NAME`_MD_PTR_APP_VERIFIED;
                        }

                    packetBuffer[`$INSTANCE_NAME`_DATA_ADDR] = 1 + ~checksum;
                    ackCode = CYRET_SUCCESS;
                    rspSize = 1u;
                }
                break;


            /***************************************************************************
            *   Exit Bootloader
            ***************************************************************************/
            case `$INSTANCE_NAME`_COMMAND_EXIT:

                if(!`$INSTANCE_NAME`_ValidateApp(`$INSTANCE_NAME`_activeApp))
                {
                    `$INSTANCE_NAME`_SET_RUN_TYPE(`$INSTANCE_NAME`_START_APP);
                }

                `$INSTANCE_NAME`_SOFTWARE_RESET;

                /* Will never get here */
                continue;

                /* Will never get here. Break is intentionally removed. */


            /***************************************************************************
            *   Unsupported Command
            ***************************************************************************/
            default:
                ackCode = `$INSTANCE_NAME`_ERR_CMD;
                break;
            }
        }

        /* ?CK the packet and function. */
        `$INSTANCE_NAME`_WritePacket(ackCode, packetBuffer, rspSize);

    } while (communicationActive);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_WritePacket
********************************************************************************
*
* Summary:
*  Creates a bootloader responce packet and transmits it back to the bootloader
*  host application over the already established communications protocol.
*
* Parameters:
*  status:
*      The status code to pass back as the second byte of the packet
*  buffer:
*      The buffer containing the data portion of the packet
*  size:
*      The number of bytes contained within the buffer to pass back
*
* Return:
*   CYRET_SUCCESS if successful.
*   CYRET_UNKNOWN if there was an error tranmitting the packet.
*
*******************************************************************************/
int `$INSTANCE_NAME`_WritePacket(uint8 status, uint8 CYXDATA* buffer, uint16 size) CYSMALL `=ReentrantKeil("`$INSTANCE_NAME`_WritePacket")`
{
    uint16 CYDATA checksum;

    /* Start of the packet. */
    buffer[`$INSTANCE_NAME`_SOP_ADDR]      = `$INSTANCE_NAME`_SOP;
    buffer[`$INSTANCE_NAME`_CMD_ADDR]      = status;
    buffer[`$INSTANCE_NAME`_SIZE_ADDR]     = LO8(size);
    buffer[`$INSTANCE_NAME`_SIZE_ADDR + 1] = HI8(size);

    /* Compute the checksum. */
    checksum = `$INSTANCE_NAME`_CalcPacketChecksum(buffer, size + `$INSTANCE_NAME`_DATA_ADDR);

    buffer[`$INSTANCE_NAME`_CHK_ADDR(size)]     = LO8(checksum);
    buffer[`$INSTANCE_NAME`_CHK_ADDR(1 + size)] = HI8(checksum);
    buffer[`$INSTANCE_NAME`_EOP_ADDR(size)]     = `$INSTANCE_NAME`_EOP;

    /* Start the packet transmit. */
    return(CyBtldrCommWrite(buffer, size + `$INSTANCE_NAME`_MIN_PKT_SIZE, &size, 150));
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetFlashByte
********************************************************************************
*
* Summary:
*  Writes byte a flash memory location
*
* Parameters:
*  address:
*      Address in Flash memory where data will be written
*
*  runType:
*      Byte to be written
*
* Return:
*  None
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetFlashByte(uint32 address, uint8 runType) `=ReentrantKeil("`$INSTANCE_NAME`_SetFlashByte")`
{
    uint32 flsAddr = address - CYDEV_FLASH_BASE;
    uint8  rowData[CYDEV_FLS_ROW_SIZE];

    #if !(CY_PSOC4)
        uint8 arrayId = (flsAddr / CYDEV_FLS_SECTOR_SIZE);
    #endif  /* !(CY_PSOC4) */

    uint16 rowNum = ((flsAddr % CYDEV_FLS_SECTOR_SIZE) / CYDEV_FLS_ROW_SIZE);
    uint32 baseAddr = address - (address % CYDEV_FLS_ROW_SIZE);
    uint16 idx;

    for(idx = 0; idx < CYDEV_FLS_ROW_SIZE; idx++)
    {
        rowData[idx] = `$INSTANCE_NAME`_GET_CODE_DATA(baseAddr + idx);
    }

    rowData[address % CYDEV_FLS_ROW_SIZE] = runType;

    #if(CY_PSOC4)
        CySysFlashWriteRow(rowNum, rowData);
    #else
        CyWriteRowData(arrayId, rowNum, rowData);
    #endif  /* (CY_PSOC4) */
}


/* [] END OF FILE */
