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
    /// <summary>
    /// Wrapper for tabs
    /// </summary>
    public class CyEditingWrapperControl : UserControl, ICyParamEditingControl
    {
        public static bool RUN_MODE = false;
        protected CyParameters m_parameters = null;

        public virtual string TabName
        {
            get { return "Empty"; }
        }

        public CyEditingWrapperControl()
        {
            this.Load += new EventHandler(CyEditingWrapperControl_Load);
        }

        void CyEditingWrapperControl_Load(object sender, EventArgs e)
        {
            this.AutoScroll = true;
            if (RUN_MODE)
                this.Dock = DockStyle.Fill;
        }

        #region ICyParamEditingControl Members
        public Control DisplayControl
        {
            get { return this; }
        }

        // Gets any errors that exist for parameters on the DisplayControl.
        public virtual IEnumerable<CyCustErr> GetErrors()
        {
            foreach (string paramName in m_parameters.m_inst.GetParamNames())
            {
                CyCompDevParam param = m_parameters.m_inst.GetCommittedParam(paramName);
                if (param.TabName.Equals(TabName))
                {
                    if (param.ErrorCount > 0)
                    {
                        foreach (string errMsg in param.Errors)
                        {
                            yield return new CyCustErr(errMsg);
                        }
                    }
                }
            }

        }
        #endregion
    }
}
