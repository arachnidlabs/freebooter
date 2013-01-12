/*******************************************************************************
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
********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using CyDesigner.Extensions.Gde;
using CyDesigner.Extensions.Common;

namespace Freebooter_v1_0
{
    [CyCompDevCustomizer]
    public partial class CyCustomizer : ICyParamEditHook_v1, ICyBootloaderProvider_v1, ICyDRCProvider_v1
    {
        #region ICyParamEditHook_v1 Members
        DialogResult ICyParamEditHook_v1.EditParams(ICyInstEdit_v1 edit, ICyTerminalQuery_v1 termQuery,
                                                    ICyExpressMgr_v1 mgr)
        {
            const string PAPAM_TAB_NAME_BUILTIN = "Built-in";
            CyEditingWrapperControl.RUN_MODE = true;
            CyParameters prms = new CyParameters(edit);
            CyGeneralPage generalPage = new CyGeneralPage(prms);
            ICyTabbedParamEditor editor = edit.CreateTabbedParamEditor();

            CyParamExprDelegate exprDelegate = delegate(ICyParamEditor custEditor, CyCompDevParam param)
            {
                prms.m_globalEditMode = false;
                if (param.TabName == generalPage.TabName)
                    generalPage.InitFields();
                prms.m_globalEditMode = true;
            };

            editor.AddCustomPage(Properties.Resources.PageTitleGeneral, generalPage, exprDelegate, generalPage.TabName);
            editor.AddDefaultPage(Properties.Resources.PageTitleBuiltIn, PAPAM_TAB_NAME_BUILTIN);

            prms.m_globalEditMode = true;

            return editor.ShowDialog();
        }

        bool ICyParamEditHook_v1.EditParamsOnDrop
        {
            get { return false; }
        }

        CyCompDevParamEditorMode ICyParamEditHook_v1.GetEditorMode()
        {
            return CyCompDevParamEditorMode.COMPLETE;
        }
        #endregion

        #region ICyBootloaderProvider_v1 Members
        public CyCustErr GetBootloaderData(ICyBootloaderProviderArgs_v1 args, out CyBootloaderData_v1 data)
        {
            CyParameters prms = new CyParameters(args.InstQuery);

            CyBootloaderChecksumType_v1 checksum = CyBootloaderChecksumType_v1.BasicChecksum;
            if (prms.PacketChecksumType == true)
            {
                checksum = CyBootloaderChecksumType_v1.CRC16;
            }

            data = new CyBootloaderData_v1(prms.Version, checksum, prms.IOComponent);
            
            return CyCustErr.OK;
        }
        #endregion

        #region ICyDRCProvider_v1 Members
        public IEnumerable<CyDRCInfo_v1> GetDRCs(ICyDRCProviderArgs_v1 args)
        {
            CyParameters parameters = new CyParameters(args.InstQueryV1);

            if (parameters.CompareAppType() == false)
            {
                yield return new CyDRCInfo_v1(CyDRCInfo_v1.CyDRCType_v1.Error, 
                                              Properties.Resources.AppTypeCompareError);
            }

        }
        #endregion
    }
}
