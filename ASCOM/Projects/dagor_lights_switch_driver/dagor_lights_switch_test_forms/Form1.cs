using System;
using System.Windows.Forms;

namespace ASCOM.DagorLights
{
    public partial class Form1 : Form
    {

        private ASCOM.DriverAccess.Switch driver;

        public Form1()
        {
            InitializeComponent();
            SetUIState();
         
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

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void checkBoxLight1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxLight1.Checked)
            {

                driver.SetSwitch(0, true);

                checkBoxLight1.Text = "ON";
                checkBoxLight1.BackColor = System.Drawing.Color.Green;
               
            }
            else
            {
                driver.SetSwitch(0, false);

                checkBoxLight1.Text = "OFF";
                checkBoxLight1.BackColor = System.Drawing.Color.DarkRed;
            }
        }

        private void checkBoxLight2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxLight2.Checked)
            {
                driver.SetSwitch(1, true);

                checkBoxLight2.Text = "ON";
                checkBoxLight2.BackColor = System.Drawing.Color.Green;
            }
            else
            {
                driver.SetSwitch(1, false);

                checkBoxLight2.Text = "OFF";
                checkBoxLight2.BackColor = System.Drawing.Color.DarkRed;
            }
        }
    }
}
