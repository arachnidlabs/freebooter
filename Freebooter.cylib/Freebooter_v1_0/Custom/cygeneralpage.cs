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
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using CyDesigner.Extensions.Gde;
using CyDesigner.Extensions.Common;
using System.Diagnostics;

namespace Freebooter_v1_0
{
    public partial class CyGeneralPage : CyEditingWrapperControl
    {
        enum CyPacketChecksumType { BASIC_SUM, CRC_16 };

        #region Const
        const uint MIN_VAL = 0;
        const uint MAX_VAL_16 = 0xFFFF;
        const string MAX_VAL_16_STR = "0xFFFF";
        const uint MAX_VAL_WAITTIME = 255;
        #endregion Const

        List<string> m_communicationComponentList = new List<string>();

        #region Constructors
        public CyGeneralPage()
        {
            InitializeComponent();
        }

        public CyGeneralPage(CyParameters parameters)
        {
            InitializeComponent();
            m_parameters = parameters;
            InitFields();
        }
        #endregion Constructors

        #region Initialization
        public void InitFields()
        {
            checkBoxWaitTime.Checked = m_parameters.WaitEnable;
            numUpDownWaitTime.Value = m_parameters.WaitTime * 10;
            textBoxVersion.Text = "0x" + m_parameters.Version.ToString("X4");

            comboBoxPktChecksumType.SelectedIndex = m_parameters.PacketChecksumType ?
                    (byte)CyPacketChecksumType.CRC_16 : (byte)CyPacketChecksumType.BASIC_SUM;

            checkBoxBeAppValidation.Checked = m_parameters.FastValidation;
            checkBoxBrAppValidation.Checked = m_parameters.FastBrValidation;

            checkBoxCommSize.Checked = m_parameters.CmdFlashSize;
            checkBoxCommErase.Checked = m_parameters.CmdEraseRow;
            checkBoxCommVerify.Checked = m_parameters.CmdVerifyRow;
            checkBoxCommSend.Checked = m_parameters.CmdSendData;
            checkBoxCommSync.Checked = m_parameters.CmdSyncBootloader;
            checkBoxCommStatus.Checked = m_parameters.CmdGetAppStatus;

            checkBoxMultiApp.Checked = m_parameters.MultiAppBootloader;
            CheckMultiApp();

            m_communicationComponentList = new List<string>();
            m_communicationComponentList.Add(CyParameters.CUSTOM_INTERFACE);
            try
            {
                m_communicationComponentList.AddRange(
                    m_parameters.m_inst.DesignQuery.GetBootloaderCommunicationComponents());
            }
            catch
            {
                Debug.Assert(false);
            }
            comboBoxIOComponent.Items.Clear();
            comboBoxIOComponent.Items.AddRange(m_communicationComponentList.ToArray());
            if (comboBoxIOComponent.Items.Contains(m_parameters.IOComponent))
            {
                comboBoxIOComponent.SelectedItem = m_parameters.IOComponent;
            }
            else
            {
                comboBoxIOComponent.SelectedIndex = 0;
            }

        }
        #endregion Initialization

        #region Event handlers

        private void textBoxVersion_Validated(object sender, EventArgs e)
        {
            TextBox tb = (TextBox)sender;
            uint val;
            if (!IsHex(tb.Text))
            {
                errProvider.SetError(tb, String.Format(Properties.Resources.ErrTBFormat, 2));
            }
            else
            {
                if (TryParseHex(tb.Text, out val))
                {
                    if (!CheckRange(val, MIN_VAL, MAX_VAL_16))
                    {
                        errProvider.SetError(tb, String.Format(Properties.Resources.ErrTBRange,
                                                               MIN_VAL, MAX_VAL_16_STR));
                    }
                    else
                    {
                        errProvider.SetError(tb, "");
                        m_parameters.Version = (UInt16)val;
                    }
                }
                else
                {
                    errProvider.SetError(tb, String.Format(Properties.Resources.ErrTBRange, MIN_VAL, MAX_VAL_16_STR));
                }
            }
        }

        private void checkBoxOptions_CheckedChanged(object sender, EventArgs e)
        {
            bool val = ((CheckBox)sender).Checked;
            if (sender == checkBoxBeAppValidation)
                m_parameters.FastValidation = val;
            if (sender == checkBoxBrAppValidation)
                m_parameters.FastBrValidation = val;
        }

        private void checkBoxComm_CheckedChanged(object sender, EventArgs e)
        {
            bool val = ((CheckBox)sender).Checked;
            if (sender == checkBoxCommSize)
                m_parameters.CmdFlashSize = val;
            else if (sender == checkBoxCommErase)
                m_parameters.CmdEraseRow = val;
            else if (sender == checkBoxCommVerify)
                m_parameters.CmdVerifyRow = val;
            else if (sender == checkBoxCommSend)
                m_parameters.CmdSendData = val;
            else if (sender == checkBoxCommSync)
                m_parameters.CmdSyncBootloader = val;
            else if (sender == checkBoxCommStatus)
                m_parameters.CmdGetAppStatus = val;
        }


        private void checkBoxWaitTime_CheckedChanged(object sender, EventArgs e)
        {
            labelWaitTime.Enabled = checkBoxWaitTime.Checked;
            numUpDownWaitTime.Enabled = checkBoxWaitTime.Checked;
            labelZero.Enabled = checkBoxWaitTime.Checked;

            m_parameters.WaitEnable = checkBoxWaitTime.Checked;
        }

        private void comboBoxPktChecksumType_SelectedIndexChanged(object sender, EventArgs e)
        {
            bool res = false;
            if (comboBoxPktChecksumType.SelectedIndex == (byte)CyPacketChecksumType.CRC_16)
                res = true;
            m_parameters.PacketChecksumType = res;
        }

        private void numUpDownWaitTime_ValueChanged(object sender, EventArgs e)
        {
            if ((int)numUpDownWaitTime.Value % 10 != 0)
            {
                errProvider.SetError(numUpDownWaitTime, Properties.Resources.ErrWaitTime10x);
            }
            else
            {
                errProvider.SetError(numUpDownWaitTime, "");
                m_parameters.WaitTime = (byte)((int)numUpDownWaitTime.Value / 10);
            }
        }

        private void checkBoxMultiApp_CheckedChanged(object sender, EventArgs e)
        {
            checkBoxCommStatus.Enabled = checkBoxMultiApp.Checked;
            m_parameters.MultiAppBootloader = checkBoxMultiApp.Checked;
            CheckMultiApp();
        }

        private void comboBoxIOComponent_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_parameters.IOComponent = comboBoxIOComponent.Text;
        }
        #endregion Event handlers

        #region Service functions
        public static bool IsHex(string value)
        {
            Regex hexTemplate = new Regex("^(0x)?[0-9a-fA-F]{1,}$");
            return hexTemplate.IsMatch(value);
        }

        public static bool TryParseHex(string value, out uint res)
        {
            bool success = false;
            res = 0;
            try
            {
                res = Convert.ToUInt32(value.Replace("0x", ""), 16);
                success = true;
            }
            catch
            {
            }
            return success;
        }

        public static bool CheckRange(uint value, uint min, uint max)
        {
            return ((value >= min) && (value <= max));
        }

        private void CheckMultiApp()
        {
            if (m_parameters.CompareAppType() == false)
            {
                errProvider.SetError(checkBoxMultiApp, Properties.Resources.AppTypeCompareError);
            }
            else
            {
                errProvider.SetError(checkBoxMultiApp, "");
            }
        }
        #endregion Service functions

        #region GetErrors() override
        public override IEnumerable<CyCustErr> GetErrors()
        {
            List<CyCustErr> errorList = new List<CyCustErr>();
            errorList.AddRange(base.GetErrors());
            if (!String.IsNullOrEmpty(errProvider.GetError(numUpDownWaitTime)) ||
                !String.IsNullOrEmpty(errProvider.GetError(textBoxVersion)))
            {
                errorList.Add(new CyCustErr(Properties.Resources.ErrTBWrongValue));
            }
            if (!String.IsNullOrEmpty(errProvider.GetError(checkBoxMultiApp)))
            {
                errorList.Add(new CyCustErr(errProvider.GetError(checkBoxMultiApp)));
            }
            return errorList.ToArray();
        }

        public override string TabName
        {
            get { return "General"; }
        }
        #endregion GetErrors() override

        #region 'Enter' key override
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Enter)
            {
                SendKeys.Send("{TAB}");
                return true;
            }
            else
                return base.ProcessCmdKey(ref msg, keyData);
        }
        #endregion 'Enter' key override
                               
    }
}
