using System;
using System.Windows.Forms;

namespace ASCOM.DagorFans
{
    public partial class Form1 : Form
    {

        private ASCOM.DriverAccess.Switch driver;

        public Form1()
        {
            InitializeComponent();
            SetUIState();

            // TODO: get state for each fan

            // TODO: Initialise radio button states
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (IsConnected)
                driver.Connected = false;

            Properties.Settings.Default.Save();
        }

        private void buttonChoose_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.DriverId = ASCOM.DriverAccess.Switch.Choose(Properties.Settings.Default.DriverId);
            SetUIState();
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (IsConnected)
            {
                driver.Connected = false;
            }
            else
            {
                driver = new ASCOM.DriverAccess.Switch(Properties.Settings.Default.DriverId);
                driver.Connected = true;
            }
            SetUIState();
        }

        private void SetUIState()
        {
            buttonConnect.Enabled = !string.IsNullOrEmpty(Properties.Settings.Default.DriverId);
            buttonChoose.Enabled = !IsConnected;
            buttonConnect.Text = IsConnected ? "Disconnect" : "Connect";
        }

        private bool IsConnected
        {
            get
            {
                return ((this.driver != null) && (driver.Connected == true));
            }
        }

        #region Fan 1

        private void radioBtnFan1Off_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnFan1Off.Checked)
            {                
                radioBtnFan1Off.BackColor = System.Drawing.Color.DarkRed;

                // TODO: Turn Off Fan 1
            }
            else
            {
                radioBtnFan1Off.BackColor = default(System.Drawing.Color);
            }
        }

        private void radioBtnFan1Speed1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnFan1Speed1.Checked)
            {
                radioBtnFan1Speed1.BackColor = System.Drawing.Color.Green;

                // TODO: Set to speed 1
            }
            else
            {
                radioBtnFan1Speed1.BackColor = default(System.Drawing.Color);
            }
        }

        private void radioBtnFan1Speed2_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnFan1Speed2.Checked)
            {
                radioBtnFan1Speed2.BackColor = System.Drawing.Color.Green;

                // TODO: Set to speed 2
            }
            else
            {
                radioBtnFan1Speed2.BackColor = default(System.Drawing.Color);
            }
        }

        #endregion

        #region Fan 2

        private void radioBtnFan2Off_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnFan2Off.Checked)
            {                
                radioBtnFan2Off.BackColor = System.Drawing.Color.DarkRed;

                // TODO: Turn Off Fan 2
            }
            else
            {
                radioBtnFan2Off.BackColor = default(System.Drawing.Color);
            }
        }

        private void radioBtnFan2Speed1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnFan2Speed1.Checked)
            {
                radioBtnFan2Speed1.BackColor = System.Drawing.Color.Green;

                // TODO: Set to speed 1
            }
            else
            {
                radioBtnFan2Speed1.BackColor = default(System.Drawing.Color);
            }
        }

        private void radioBtnFan2Speed2_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnFan2Speed2.Checked)
            {
                radioBtnFan2Speed2.BackColor = System.Drawing.Color.Green;

                // TODO: Set to speed 2
            }
            else
            {
                radioBtnFan2Speed2.BackColor = default(System.Drawing.Color);
            }
        }

        #endregion

        #region Fan 3

        private void radioBtnFan3Off_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnFan3Off.Checked)
            {                
                radioBtnFan3Off.BackColor = System.Drawing.Color.DarkRed;

                // TODO: Turn Off Fan 3
            }
            else
            {
                radioBtnFan3Off.BackColor = default(System.Drawing.Color);
            }
        }

        private void radioBtnFan3Speed1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnFan3Speed1.Checked)
            {
                radioBtnFan3Speed1.BackColor = System.Drawing.Color.Green;

                // TODO: Set to speed 1
            }
            else
            {
                radioBtnFan3Speed1.BackColor = default(System.Drawing.Color);
            }
        }

        private void radioBtnFan3Speed2_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtnFan3Speed2.Checked)
            {
                radioBtnFan3Speed2.BackColor = System.Drawing.Color.Green;
                
                // TODO: Set to speed 2
            }
            else
            {
                radioBtnFan3Speed2.BackColor = default(System.Drawing.Color);
            }
        }

        #endregion

    }
}
