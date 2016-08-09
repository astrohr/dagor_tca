namespace ASCOM.DagorFans
{
    partial class Form1
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonChoose = new System.Windows.Forms.Button();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.labelDriverId = new System.Windows.Forms.Label();
            this.radioBtnFan1Off = new System.Windows.Forms.RadioButton();
            this.radioBtnFan1Speed1 = new System.Windows.Forms.RadioButton();
            this.radioBtnFan1Speed2 = new System.Windows.Forms.RadioButton();
            this.radioBtnFan2Off = new System.Windows.Forms.RadioButton();
            this.radioBtnFan2Speed1 = new System.Windows.Forms.RadioButton();
            this.radioBtnFan2Speed2 = new System.Windows.Forms.RadioButton();
            this.groupBoxFan1 = new System.Windows.Forms.GroupBox();
            this.groupBoxFan2 = new System.Windows.Forms.GroupBox();
            this.groupBoxFan1.SuspendLayout();
            this.groupBoxFan2.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonChoose
            // 
            this.buttonChoose.Location = new System.Drawing.Point(309, 10);
            this.buttonChoose.Name = "buttonChoose";
            this.buttonChoose.Size = new System.Drawing.Size(72, 23);
            this.buttonChoose.TabIndex = 0;
            this.buttonChoose.Text = "Choose";
            this.buttonChoose.UseVisualStyleBackColor = true;
            this.buttonChoose.Click += new System.EventHandler(this.buttonChoose_Click);
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(309, 39);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(72, 23);
            this.buttonConnect.TabIndex = 1;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // labelDriverId
            // 
            this.labelDriverId.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.labelDriverId.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::ASCOM.DagorFans.Properties.Settings.Default, "DriverId", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.labelDriverId.Location = new System.Drawing.Point(12, 40);
            this.labelDriverId.Name = "labelDriverId";
            this.labelDriverId.Size = new System.Drawing.Size(291, 21);
            this.labelDriverId.TabIndex = 2;
            this.labelDriverId.Text = global::ASCOM.DagorFans.Properties.Settings.Default.DriverId;
            this.labelDriverId.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // radioBtnFan1Off
            // 
            this.radioBtnFan1Off.Appearance = System.Windows.Forms.Appearance.Button;
            this.radioBtnFan1Off.AutoSize = true;
            this.radioBtnFan1Off.Location = new System.Drawing.Point(19, 26);
            this.radioBtnFan1Off.Name = "radioBtnFan1Off";
            this.radioBtnFan1Off.Size = new System.Drawing.Size(40, 23);
            this.radioBtnFan1Off.TabIndex = 6;
            this.radioBtnFan1Off.TabStop = true;
            this.radioBtnFan1Off.Text = "OFF";
            this.radioBtnFan1Off.UseVisualStyleBackColor = true;
            this.radioBtnFan1Off.CheckedChanged += new System.EventHandler(this.radioBtnFan1Off_CheckedChanged);
            // 
            // radioBtnFan1Speed1
            // 
            this.radioBtnFan1Speed1.Appearance = System.Windows.Forms.Appearance.Button;
            this.radioBtnFan1Speed1.AutoSize = true;
            this.radioBtnFan1Speed1.Location = new System.Drawing.Point(19, 55);
            this.radioBtnFan1Speed1.Name = "radioBtnFan1Speed1";
            this.radioBtnFan1Speed1.Size = new System.Drawing.Size(64, 23);
            this.radioBtnFan1Speed1.TabIndex = 7;
            this.radioBtnFan1Speed1.TabStop = true;
            this.radioBtnFan1Speed1.Text = "Speed 1";
            this.radioBtnFan1Speed1.UseVisualStyleBackColor = true;
            this.radioBtnFan1Speed1.CheckedChanged += new System.EventHandler(this.radioBtnFan1Speed1_CheckedChanged);
            // 
            // radioBtnFan1Speed2
            // 
            this.radioBtnFan1Speed2.Appearance = System.Windows.Forms.Appearance.Button;
            this.radioBtnFan1Speed2.AutoSize = true;
            this.radioBtnFan1Speed2.Location = new System.Drawing.Point(19, 84);
            this.radioBtnFan1Speed2.Name = "radioBtnFan1Speed2";
            this.radioBtnFan1Speed2.Size = new System.Drawing.Size(64, 23);
            this.radioBtnFan1Speed2.TabIndex = 8;
            this.radioBtnFan1Speed2.TabStop = true;
            this.radioBtnFan1Speed2.Text = "Speed 2";
            this.radioBtnFan1Speed2.UseVisualStyleBackColor = true;
            this.radioBtnFan1Speed2.CheckedChanged += new System.EventHandler(this.radioBtnFan1Speed2_CheckedChanged);
            // 
            // radioBtnFan2Off
            // 
            this.radioBtnFan2Off.Appearance = System.Windows.Forms.Appearance.Button;
            this.radioBtnFan2Off.AutoSize = true;
            this.radioBtnFan2Off.Location = new System.Drawing.Point(18, 26);
            this.radioBtnFan2Off.Name = "radioBtnFan2Off";
            this.radioBtnFan2Off.Size = new System.Drawing.Size(40, 23);
            this.radioBtnFan2Off.TabIndex = 10;
            this.radioBtnFan2Off.TabStop = true;
            this.radioBtnFan2Off.Text = "OFF";
            this.radioBtnFan2Off.UseVisualStyleBackColor = true;
            this.radioBtnFan2Off.CheckedChanged += new System.EventHandler(this.radioBtnFan2Off_CheckedChanged);
            // 
            // radioBtnFan2Speed1
            // 
            this.radioBtnFan2Speed1.Appearance = System.Windows.Forms.Appearance.Button;
            this.radioBtnFan2Speed1.AutoSize = true;
            this.radioBtnFan2Speed1.Location = new System.Drawing.Point(18, 55);
            this.radioBtnFan2Speed1.Name = "radioBtnFan2Speed1";
            this.radioBtnFan2Speed1.Size = new System.Drawing.Size(64, 23);
            this.radioBtnFan2Speed1.TabIndex = 11;
            this.radioBtnFan2Speed1.TabStop = true;
            this.radioBtnFan2Speed1.Text = "Speed 1";
            this.radioBtnFan2Speed1.UseVisualStyleBackColor = true;
            this.radioBtnFan2Speed1.CheckedChanged += new System.EventHandler(this.radioBtnFan2Speed1_CheckedChanged);
            // 
            // radioBtnFan2Speed2
            // 
            this.radioBtnFan2Speed2.Appearance = System.Windows.Forms.Appearance.Button;
            this.radioBtnFan2Speed2.AutoSize = true;
            this.radioBtnFan2Speed2.Location = new System.Drawing.Point(18, 84);
            this.radioBtnFan2Speed2.Name = "radioBtnFan2Speed2";
            this.radioBtnFan2Speed2.Size = new System.Drawing.Size(64, 23);
            this.radioBtnFan2Speed2.TabIndex = 12;
            this.radioBtnFan2Speed2.TabStop = true;
            this.radioBtnFan2Speed2.Text = "Speed 2";
            this.radioBtnFan2Speed2.UseVisualStyleBackColor = true;
            this.radioBtnFan2Speed2.CheckedChanged += new System.EventHandler(this.radioBtnFan2Speed2_CheckedChanged);
            // 
            // groupBoxFan1
            // 
            this.groupBoxFan1.Controls.Add(this.radioBtnFan1Speed1);
            this.groupBoxFan1.Controls.Add(this.radioBtnFan1Off);
            this.groupBoxFan1.Controls.Add(this.radioBtnFan1Speed2);
            this.groupBoxFan1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBoxFan1.Location = new System.Drawing.Point(12, 89);
            this.groupBoxFan1.Name = "groupBoxFan1";
            this.groupBoxFan1.Size = new System.Drawing.Size(102, 120);
            this.groupBoxFan1.TabIndex = 18;
            this.groupBoxFan1.TabStop = false;
            this.groupBoxFan1.Text = "Central Fan";
            // 
            // groupBoxFan2
            // 
            this.groupBoxFan2.Controls.Add(this.radioBtnFan2Speed1);
            this.groupBoxFan2.Controls.Add(this.radioBtnFan2Off);
            this.groupBoxFan2.Controls.Add(this.radioBtnFan2Speed2);
            this.groupBoxFan2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBoxFan2.Location = new System.Drawing.Point(137, 89);
            this.groupBoxFan2.Name = "groupBoxFan2";
            this.groupBoxFan2.Size = new System.Drawing.Size(100, 120);
            this.groupBoxFan2.TabIndex = 19;
            this.groupBoxFan2.TabStop = false;
            this.groupBoxFan2.Text = "Other fans";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(389, 218);
            this.Controls.Add(this.groupBoxFan2);
            this.Controls.Add(this.groupBoxFan1);
            this.Controls.Add(this.labelDriverId);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.buttonChoose);
            this.Name = "Form1";
            this.Text = "ASCOM Fans Test From";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.groupBoxFan1.ResumeLayout(false);
            this.groupBoxFan1.PerformLayout();
            this.groupBoxFan2.ResumeLayout(false);
            this.groupBoxFan2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonChoose;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Label labelDriverId;
        private System.Windows.Forms.RadioButton radioBtnFan1Off;
        private System.Windows.Forms.RadioButton radioBtnFan1Speed1;
        private System.Windows.Forms.RadioButton radioBtnFan1Speed2;
        private System.Windows.Forms.RadioButton radioBtnFan2Off;
        private System.Windows.Forms.RadioButton radioBtnFan2Speed1;
        private System.Windows.Forms.RadioButton radioBtnFan2Speed2;
        private System.Windows.Forms.GroupBox groupBoxFan1;
        private System.Windows.Forms.GroupBox groupBoxFan2;
    }
}

