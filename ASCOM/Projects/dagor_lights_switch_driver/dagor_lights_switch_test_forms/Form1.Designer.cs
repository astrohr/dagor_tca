namespace ASCOM.DagorLights
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
            this.labelLight1 = new System.Windows.Forms.Label();
            this.labelLight2 = new System.Windows.Forms.Label();
            this.checkBoxLight1 = new System.Windows.Forms.CheckBox();
            this.checkBoxLight2 = new System.Windows.Forms.CheckBox();
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
            this.labelDriverId.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::ASCOM.DagorLights.Properties.Settings.Default, "DriverId", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.labelDriverId.Location = new System.Drawing.Point(12, 40);
            this.labelDriverId.Name = "labelDriverId";
            this.labelDriverId.Size = new System.Drawing.Size(291, 21);
            this.labelDriverId.TabIndex = 2;
            this.labelDriverId.Text = global::ASCOM.DagorLights.Properties.Settings.Default.DriverId;
            this.labelDriverId.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // labelLight1
            // 
            this.labelLight1.AutoSize = true;
            this.labelLight1.Location = new System.Drawing.Point(12, 97);
            this.labelLight1.Name = "labelLight1";
            this.labelLight1.Size = new System.Drawing.Size(42, 13);
            this.labelLight1.TabIndex = 3;
            this.labelLight1.Text = "Light 1:";
            // 
            // labelLight2
            // 
            this.labelLight2.AutoSize = true;
            this.labelLight2.Location = new System.Drawing.Point(12, 143);
            this.labelLight2.Name = "labelLight2";
            this.labelLight2.Size = new System.Drawing.Size(42, 13);
            this.labelLight2.TabIndex = 4;
            this.labelLight2.Text = "Light 2:";
            // 
            // checkBoxLight1
            // 
            this.checkBoxLight1.Appearance = System.Windows.Forms.Appearance.Button;
            this.checkBoxLight1.AutoSize = true;
            this.checkBoxLight1.BackColor = System.Drawing.Color.DarkRed;
            this.checkBoxLight1.Location = new System.Drawing.Point(72, 92);
            this.checkBoxLight1.Name = "checkBoxLight1";
            this.checkBoxLight1.Size = new System.Drawing.Size(37, 23);
            this.checkBoxLight1.TabIndex = 5;
            this.checkBoxLight1.Text = "OFF";
            this.checkBoxLight1.UseVisualStyleBackColor = false;
            this.checkBoxLight1.CheckedChanged += new System.EventHandler(this.checkBoxLight1_CheckedChanged);
            // 
            // checkBoxLight2
            // 
            this.checkBoxLight2.Appearance = System.Windows.Forms.Appearance.Button;
            this.checkBoxLight2.AutoSize = true;
            this.checkBoxLight2.BackColor = System.Drawing.Color.DarkRed;
            this.checkBoxLight2.Location = new System.Drawing.Point(72, 138);
            this.checkBoxLight2.Name = "checkBoxLight2";
            this.checkBoxLight2.Size = new System.Drawing.Size(37, 23);
            this.checkBoxLight2.TabIndex = 6;
            this.checkBoxLight2.Text = "OFF";
            this.checkBoxLight2.UseVisualStyleBackColor = false;
            this.checkBoxLight2.CheckedChanged += new System.EventHandler(this.checkBoxLight2_CheckedChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(388, 262);
            this.Controls.Add(this.checkBoxLight2);
            this.Controls.Add(this.checkBoxLight1);
            this.Controls.Add(this.labelLight2);
            this.Controls.Add(this.labelLight1);
            this.Controls.Add(this.labelDriverId);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.buttonChoose);
            this.Name = "Form1";
            this.Text = "TEMPLATEDEVICETYPE Test";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonChoose;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Label labelDriverId;
        private System.Windows.Forms.Label labelLight1;
        private System.Windows.Forms.Label labelLight2;
        private System.Windows.Forms.CheckBox checkBoxLight1;
        private System.Windows.Forms.CheckBox checkBoxLight2;
    }
}

