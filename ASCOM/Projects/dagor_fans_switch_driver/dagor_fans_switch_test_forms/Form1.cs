﻿using System;
using System.Windows.Forms;

namespace ASCOM.DagorFans
{
    public partial class Form1 : Form
    {

        private ASCOM.DriverAccess.Switch driver;

        const int NUM_FANS = 3;

        const int FAN_1_ID = 0;
        const int FAN_2_ID = 1;
        const int FAN_3_ID = 2;

        const double FAN_STATE_OFF = 0.0;
        const double FAN_STATE_SPEED_1 = 1.0;
        const double FAN_STATE_SPEED_2 = 2.0;


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

                // Get current state for each fan
                for (short i = 1; i <= NUM_FANS; i++)
                {
                    double state = driver.GetSwitchValue(i);
                    setRadioButtonStates(i, state);
                }
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

                // Turn Off Fan 1
                driver.SetSwitchValue(FAN_1_ID, FAN_STATE_OFF);

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

                // Set to speed 1
                driver.SetSwitchValue(FAN_1_ID, FAN_STATE_SPEED_1);
                
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

                // Set to speed 2
                driver.SetSwitchValue(FAN_1_ID, FAN_STATE_SPEED_2);
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

                // Turn Off Fan 2
                driver.SetSwitchValue(FAN_2_ID, FAN_STATE_OFF);
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

                // Set to speed 1
                driver.SetSwitchValue(FAN_2_ID, FAN_STATE_SPEED_1);
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

                // Set to speed 2
                driver.SetSwitchValue(FAN_2_ID, FAN_STATE_SPEED_2);
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

                // Turn Off Fan 3
                driver.SetSwitchValue(FAN_3_ID, FAN_STATE_OFF);
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

                // Set to speed 1
                driver.SetSwitchValue(FAN_3_ID, FAN_STATE_SPEED_1);
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

                // Set to speed 2
                driver.SetSwitchValue(FAN_3_ID, FAN_STATE_SPEED_2);
            }
            else
            {
                radioBtnFan3Speed2.BackColor = default(System.Drawing.Color);
            }
        }

        #endregion

        #region Helper functions

        private void setRadioButtonStates(int fanId, double state)
        {
            switch (fanId)
            {
                case FAN_1_ID:

                    if (state == FAN_STATE_OFF)
                    {
                        radioBtnFan1Off.Checked = true;
                        radioBtnFan1Off.BackColor = System.Drawing.Color.DarkRed;
                    }

                    else if (state == FAN_STATE_SPEED_1)
                    {
                        radioBtnFan1Speed1.Checked = true;
                        radioBtnFan1Speed1.BackColor = System.Drawing.Color.Green;
                    }

                    else
                    {
                        radioBtnFan1Speed2.Checked = true;
                        radioBtnFan1Speed2.BackColor = System.Drawing.Color.Green;
                    }

                    break;

                case FAN_2_ID:

                    if (state == FAN_STATE_OFF)
                    {
                        radioBtnFan2Off.Checked = true;
                        radioBtnFan2Off.BackColor = System.Drawing.Color.DarkRed;
                    }

                    else if (state == FAN_STATE_SPEED_1)
                    {
                        radioBtnFan2Speed1.Checked = true;
                        radioBtnFan2Speed1.BackColor = System.Drawing.Color.Green;
                    }

                    else
                    {
                        radioBtnFan2Speed2.Checked = true;
                        radioBtnFan2Speed2.BackColor = System.Drawing.Color.Green;
                    }

                    break;

                case FAN_3_ID:

                    if (state == FAN_STATE_OFF)
                    {
                        radioBtnFan3Off.Checked = true;
                        radioBtnFan3Off.BackColor = System.Drawing.Color.DarkRed;
                    }

                    else if (state == FAN_STATE_SPEED_1)
                    {
                        radioBtnFan3Speed1.Checked = true;
                        radioBtnFan3Speed1.BackColor = System.Drawing.Color.Green;
                    }

                    else
                    {
                        radioBtnFan3Speed2.Checked = true;
                        radioBtnFan3Speed2.BackColor = System.Drawing.Color.Green;
                    }

                    break;
            }
        }

        #endregion
    }
}