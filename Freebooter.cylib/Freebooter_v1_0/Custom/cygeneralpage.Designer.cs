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

namespace Freebooter_v1_0
{
    partial class CyGeneralPage
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.errProvider = new System.Windows.Forms.ErrorProvider(this.components);
            this.groupBoxOptions = new System.Windows.Forms.GroupBox();
            this.checkBoxBrAppValidation = new System.Windows.Forms.CheckBox();
            this.comboBoxIOComponent = new System.Windows.Forms.ComboBox();
            this.labelIOComponent = new System.Windows.Forms.Label();
            this.checkBoxMultiApp = new System.Windows.Forms.CheckBox();
            this.labelZero = new System.Windows.Forms.Label();
            this.checkBoxBeAppValidation = new System.Windows.Forms.CheckBox();
            this.numUpDownWaitTime = new System.Windows.Forms.NumericUpDown();
            this.labelPktChecksumType = new System.Windows.Forms.Label();
            this.comboBoxPktChecksumType = new System.Windows.Forms.ComboBox();
            this.checkBoxWaitTime = new System.Windows.Forms.CheckBox();
            this.textBoxVersion = new System.Windows.Forms.TextBox();
            this.labelVersion = new System.Windows.Forms.Label();
            this.labelWaitTime = new System.Windows.Forms.Label();
            this.groupBoxCommands = new System.Windows.Forms.GroupBox();
            this.checkBoxCommStatus = new System.Windows.Forms.CheckBox();
            this.checkBoxCommSync = new System.Windows.Forms.CheckBox();
            this.checkBoxCommSend = new System.Windows.Forms.CheckBox();
            this.checkBoxCommVerify = new System.Windows.Forms.CheckBox();
            this.checkBoxCommErase = new System.Windows.Forms.CheckBox();
            this.checkBoxCommSize = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.errProvider)).BeginInit();
            this.groupBoxOptions.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownWaitTime)).BeginInit();
            this.groupBoxCommands.SuspendLayout();
            this.SuspendLayout();
            // 
            // errProvider
            // 
            this.errProvider.BlinkStyle = System.Windows.Forms.ErrorBlinkStyle.NeverBlink;
            this.errProvider.ContainerControl = this;
            // 
            // groupBoxOptions
            // 
            this.groupBoxOptions.Controls.Add(this.checkBoxBrAppValidation);
            this.groupBoxOptions.Controls.Add(this.comboBoxIOComponent);
            this.groupBoxOptions.Controls.Add(this.labelIOComponent);
            this.groupBoxOptions.Controls.Add(this.checkBoxMultiApp);
            this.groupBoxOptions.Controls.Add(this.labelZero);
            this.groupBoxOptions.Controls.Add(this.checkBoxBeAppValidation);
            this.groupBoxOptions.Controls.Add(this.numUpDownWaitTime);
            this.groupBoxOptions.Controls.Add(this.labelPktChecksumType);
            this.groupBoxOptions.Controls.Add(this.comboBoxPktChecksumType);
            this.groupBoxOptions.Controls.Add(this.checkBoxWaitTime);
            this.groupBoxOptions.Controls.Add(this.textBoxVersion);
            this.groupBoxOptions.Controls.Add(this.labelVersion);
            this.groupBoxOptions.Controls.Add(this.labelWaitTime);
            this.groupBoxOptions.Location = new System.Drawing.Point(3, 3);
            this.groupBoxOptions.Name = "groupBoxOptions";
            this.groupBoxOptions.Size = new System.Drawing.Size(316, 230);
            this.groupBoxOptions.TabIndex = 0;
            this.groupBoxOptions.TabStop = false;
            this.groupBoxOptions.Text = "Options";
            // 
            // checkBoxBrAppValidation
            // 
            this.checkBoxBrAppValidation.AutoSize = true;
            this.checkBoxBrAppValidation.Location = new System.Drawing.Point(6, 209);
            this.checkBoxBrAppValidation.Name = "checkBoxBrAppValidation";
            this.checkBoxBrAppValidation.Size = new System.Drawing.Size(179, 17);
            this.checkBoxBrAppValidation.TabIndex = 7;
            this.checkBoxBrAppValidation.Text = "Bootloader application validation";
            this.checkBoxBrAppValidation.UseVisualStyleBackColor = true;
            this.checkBoxBrAppValidation.CheckedChanged += new System.EventHandler(this.checkBoxOptions_CheckedChanged);
            // 
            // comboBoxIOComponent
            // 
            this.comboBoxIOComponent.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxIOComponent.FormattingEnabled = true;
            this.comboBoxIOComponent.Location = new System.Drawing.Point(175, 19);
            this.comboBoxIOComponent.Name = "comboBoxIOComponent";
            this.comboBoxIOComponent.Size = new System.Drawing.Size(121, 21);
            this.comboBoxIOComponent.TabIndex = 0;
            this.comboBoxIOComponent.SelectedIndexChanged += new System.EventHandler(this.comboBoxIOComponent_SelectedIndexChanged);
            // 
            // labelIOComponent
            // 
            this.labelIOComponent.AutoSize = true;
            this.labelIOComponent.Location = new System.Drawing.Point(6, 22);
            this.labelIOComponent.Name = "labelIOComponent";
            this.labelIOComponent.Size = new System.Drawing.Size(138, 13);
            this.labelIOComponent.TabIndex = 12;
            this.labelIOComponent.Text = "Communication component:";
            // 
            // checkBoxMultiApp
            // 
            this.checkBoxMultiApp.AutoSize = true;
            this.checkBoxMultiApp.Checked = true;
            this.checkBoxMultiApp.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxMultiApp.Location = new System.Drawing.Point(6, 42);
            this.checkBoxMultiApp.Name = "checkBoxMultiApp";
            this.checkBoxMultiApp.Size = new System.Drawing.Size(155, 17);
            this.checkBoxMultiApp.TabIndex = 1;
            this.checkBoxMultiApp.Text = "Multi-application bootloader";
            this.checkBoxMultiApp.UseVisualStyleBackColor = true;
            this.checkBoxMultiApp.CheckedChanged += new System.EventHandler(this.checkBoxMultiApp_CheckedChanged);
            // 
            // labelZero
            // 
            this.labelZero.AutoSize = true;
            this.labelZero.Location = new System.Drawing.Point(170, 109);
            this.labelZero.Name = "labelZero";
            this.labelZero.Size = new System.Drawing.Size(80, 13);
            this.labelZero.TabIndex = 12;
            this.labelZero.Text = "(0: wait forever)";
            // 
            // checkBoxBeAppValidation
            // 
            this.checkBoxBeAppValidation.AutoSize = true;
            this.checkBoxBeAppValidation.Location = new System.Drawing.Point(6, 186);
            this.checkBoxBeAppValidation.Name = "checkBoxBeAppValidation";
            this.checkBoxBeAppValidation.Size = new System.Drawing.Size(212, 17);
            this.checkBoxBeAppValidation.TabIndex = 6;
            this.checkBoxBeAppValidation.Text = "Fast bootloadable application validation";
            this.checkBoxBeAppValidation.UseVisualStyleBackColor = true;
            this.checkBoxBeAppValidation.CheckedChanged += new System.EventHandler(this.checkBoxOptions_CheckedChanged);
            // 
            // numUpDownWaitTime
            // 
            this.numUpDownWaitTime.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numUpDownWaitTime.Location = new System.Drawing.Point(173, 86);
            this.numUpDownWaitTime.Maximum = new decimal(new int[] {
            2550,
            0,
            0,
            0});
            this.numUpDownWaitTime.Name = "numUpDownWaitTime";
            this.numUpDownWaitTime.Size = new System.Drawing.Size(123, 20);
            this.numUpDownWaitTime.TabIndex = 3;
            this.numUpDownWaitTime.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numUpDownWaitTime.ValueChanged += new System.EventHandler(this.numUpDownWaitTime_ValueChanged);
            // 
            // labelPktChecksumType
            // 
            this.labelPktChecksumType.AutoSize = true;
            this.labelPktChecksumType.Location = new System.Drawing.Point(3, 160);
            this.labelPktChecksumType.Name = "labelPktChecksumType";
            this.labelPktChecksumType.Size = new System.Drawing.Size(119, 13);
            this.labelPktChecksumType.TabIndex = 9;
            this.labelPktChecksumType.Text = "Packet checksum type:";
            // 
            // comboBoxPktChecksumType
            // 
            this.comboBoxPktChecksumType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPktChecksumType.FormattingEnabled = true;
            this.comboBoxPktChecksumType.Items.AddRange(new object[] {
            "Basic summation",
            "CRC-16-CCITT"});
            this.comboBoxPktChecksumType.Location = new System.Drawing.Point(173, 157);
            this.comboBoxPktChecksumType.Name = "comboBoxPktChecksumType";
            this.comboBoxPktChecksumType.Size = new System.Drawing.Size(123, 21);
            this.comboBoxPktChecksumType.TabIndex = 5;
            this.comboBoxPktChecksumType.SelectedIndexChanged += new System.EventHandler(this.comboBoxPktChecksumType_SelectedIndexChanged);
            // 
            // checkBoxWaitTime
            // 
            this.checkBoxWaitTime.AutoSize = true;
            this.checkBoxWaitTime.Checked = true;
            this.checkBoxWaitTime.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxWaitTime.Location = new System.Drawing.Point(6, 65);
            this.checkBoxWaitTime.Name = "checkBoxWaitTime";
            this.checkBoxWaitTime.Size = new System.Drawing.Size(112, 17);
            this.checkBoxWaitTime.TabIndex = 2;
            this.checkBoxWaitTime.Text = "Wait for command";
            this.checkBoxWaitTime.UseVisualStyleBackColor = true;
            this.checkBoxWaitTime.CheckedChanged += new System.EventHandler(this.checkBoxWaitTime_CheckedChanged);
            // 
            // textBoxVersion
            // 
            this.textBoxVersion.Location = new System.Drawing.Point(173, 131);
            this.textBoxVersion.Name = "textBoxVersion";
            this.textBoxVersion.Size = new System.Drawing.Size(123, 20);
            this.textBoxVersion.TabIndex = 4;
            this.textBoxVersion.Validated += new System.EventHandler(this.textBoxVersion_Validated);
            // 
            // labelVersion
            // 
            this.labelVersion.AutoSize = true;
            this.labelVersion.Location = new System.Drawing.Point(3, 134);
            this.labelVersion.Name = "labelVersion";
            this.labelVersion.Size = new System.Drawing.Size(152, 13);
            this.labelVersion.TabIndex = 0;
            this.labelVersion.Text = "Bootloader application version:";
            // 
            // labelWaitTime
            // 
            this.labelWaitTime.AutoSize = true;
            this.labelWaitTime.Location = new System.Drawing.Point(22, 88);
            this.labelWaitTime.Name = "labelWaitTime";
            this.labelWaitTime.Size = new System.Drawing.Size(140, 13);
            this.labelWaitTime.TabIndex = 0;
            this.labelWaitTime.Text = "Wait for command time (ms):";
            // 
            // groupBoxCommands
            // 
            this.groupBoxCommands.Controls.Add(this.checkBoxCommStatus);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommSync);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommSend);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommVerify);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommErase);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommSize);
            this.groupBoxCommands.Location = new System.Drawing.Point(325, 3);
            this.groupBoxCommands.Name = "groupBoxCommands";
            this.groupBoxCommands.Size = new System.Drawing.Size(141, 230);
            this.groupBoxCommands.TabIndex = 2;
            this.groupBoxCommands.TabStop = false;
            this.groupBoxCommands.Text = "Optional commands";
            // 
            // checkBoxCommStatus
            // 
            this.checkBoxCommStatus.AutoSize = true;
            this.checkBoxCommStatus.Location = new System.Drawing.Point(6, 134);
            this.checkBoxCommStatus.Name = "checkBoxCommStatus";
            this.checkBoxCommStatus.Size = new System.Drawing.Size(128, 17);
            this.checkBoxCommStatus.TabIndex = 11;
            this.checkBoxCommStatus.Text = "Get application status";
            this.checkBoxCommStatus.UseVisualStyleBackColor = true;
            this.checkBoxCommStatus.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommSync
            // 
            this.checkBoxCommSync.AutoSize = true;
            this.checkBoxCommSync.Location = new System.Drawing.Point(6, 111);
            this.checkBoxCommSync.Name = "checkBoxCommSync";
            this.checkBoxCommSync.Size = new System.Drawing.Size(103, 17);
            this.checkBoxCommSync.TabIndex = 10;
            this.checkBoxCommSync.Text = "Sync bootloader";
            this.checkBoxCommSync.UseVisualStyleBackColor = true;
            this.checkBoxCommSync.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommSend
            // 
            this.checkBoxCommSend.AutoSize = true;
            this.checkBoxCommSend.Location = new System.Drawing.Point(6, 88);
            this.checkBoxCommSend.Name = "checkBoxCommSend";
            this.checkBoxCommSend.Size = new System.Drawing.Size(75, 17);
            this.checkBoxCommSend.TabIndex = 9;
            this.checkBoxCommSend.Text = "Send data";
            this.checkBoxCommSend.UseVisualStyleBackColor = true;
            this.checkBoxCommSend.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommVerify
            // 
            this.checkBoxCommVerify.AutoSize = true;
            this.checkBoxCommVerify.Location = new System.Drawing.Point(6, 65);
            this.checkBoxCommVerify.Name = "checkBoxCommVerify";
            this.checkBoxCommVerify.Size = new System.Drawing.Size(72, 17);
            this.checkBoxCommVerify.TabIndex = 8;
            this.checkBoxCommVerify.Text = "Verify row";
            this.checkBoxCommVerify.UseVisualStyleBackColor = true;
            this.checkBoxCommVerify.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommErase
            // 
            this.checkBoxCommErase.AutoSize = true;
            this.checkBoxCommErase.Location = new System.Drawing.Point(6, 42);
            this.checkBoxCommErase.Name = "checkBoxCommErase";
            this.checkBoxCommErase.Size = new System.Drawing.Size(73, 17);
            this.checkBoxCommErase.TabIndex = 7;
            this.checkBoxCommErase.Text = "Erase row";
            this.checkBoxCommErase.UseVisualStyleBackColor = true;
            this.checkBoxCommErase.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommSize
            // 
            this.checkBoxCommSize.AutoSize = true;
            this.checkBoxCommSize.Location = new System.Drawing.Point(6, 19);
            this.checkBoxCommSize.Name = "checkBoxCommSize";
            this.checkBoxCommSize.Size = new System.Drawing.Size(89, 17);
            this.checkBoxCommSize.TabIndex = 6;
            this.checkBoxCommSize.Text = "Get flash size";
            this.checkBoxCommSize.UseVisualStyleBackColor = true;
            this.checkBoxCommSize.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // CyGeneralPage
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.Controls.Add(this.groupBoxCommands);
            this.Controls.Add(this.groupBoxOptions);
            this.Name = "CyGeneralPage";
            this.Size = new System.Drawing.Size(469, 255);
            ((System.ComponentModel.ISupportInitialize)(this.errProvider)).EndInit();
            this.groupBoxOptions.ResumeLayout(false);
            this.groupBoxOptions.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownWaitTime)).EndInit();
            this.groupBoxCommands.ResumeLayout(false);
            this.groupBoxCommands.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ErrorProvider errProvider;
        private System.Windows.Forms.GroupBox groupBoxCommands;
        private System.Windows.Forms.GroupBox groupBoxOptions;
        private System.Windows.Forms.TextBox textBoxVersion;
        private System.Windows.Forms.Label labelVersion;
        private System.Windows.Forms.Label labelWaitTime;
        private System.Windows.Forms.CheckBox checkBoxBeAppValidation;
        private System.Windows.Forms.CheckBox checkBoxCommStatus;
        private System.Windows.Forms.CheckBox checkBoxCommSync;
        private System.Windows.Forms.CheckBox checkBoxCommSend;
        private System.Windows.Forms.CheckBox checkBoxCommVerify;
        private System.Windows.Forms.CheckBox checkBoxCommErase;
        private System.Windows.Forms.CheckBox checkBoxCommSize;
        private System.Windows.Forms.Label labelPktChecksumType;
        private System.Windows.Forms.ComboBox comboBoxPktChecksumType;
        private System.Windows.Forms.CheckBox checkBoxWaitTime;
        private System.Windows.Forms.NumericUpDown numUpDownWaitTime;
        private System.Windows.Forms.Label labelZero;
        private System.Windows.Forms.ComboBox comboBoxIOComponent;
        private System.Windows.Forms.Label labelIOComponent;
        private System.Windows.Forms.CheckBox checkBoxMultiApp;
        private System.Windows.Forms.CheckBox checkBoxBrAppValidation;
    }
}
