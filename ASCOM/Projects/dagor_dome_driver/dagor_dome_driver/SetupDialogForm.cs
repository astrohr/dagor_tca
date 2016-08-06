using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using ASCOM.Utilities;
using ASCOM.Dagor;

namespace ASCOM.Dagor
{
    [ComVisible(false)]					// Form not registered for COM!
    public partial class SetupDialogForm : Form
    {
        public SetupDialogForm()
        {
            InitializeComponent();
            // Initialise current values of user settings from the ASCOM Profile
            InitUI();
        }

        private void cmdOK_Click(object sender, EventArgs e) // OK button event handler
        {
            // Place any validation constraint checks here
            // Update the state variables with results from the dialogue
            Dome.protocol = (string)comboBoxProtocol.SelectedItem;
            Dome.server = (string)textBoxServer.Text;
            Dome.port = int.Parse(textBoxPort.Text);
            Dome.tl.Enabled = chkTrace.Checked;
        }

        private void cmdCancel_Click(object sender, EventArgs e) // Cancel button event handler
        {
            Close();
        }

        private void BrowseToAscom(object sender, EventArgs e) // Click on ASCOM logo event handler
        {
            try
            {
                System.Diagnostics.Process.Start("http://ascom-standards.org/");
            }
            catch (System.ComponentModel.Win32Exception noBrowser)
            {
                if (noBrowser.ErrorCode == -2147467259)
                    MessageBox.Show(noBrowser.Message);
            }
            catch (System.Exception other)
            {
                MessageBox.Show(other.Message);
            }
        }

        private void InitUI()
        {
            chkTrace.Checked = Dome.tl.Enabled;
           
            // Set the list of available protocols to choose from
            comboBoxProtocol.Items.Clear();
            for (int i = 0; i < Dome.protocolOptions.Count; i++)
            {
                comboBoxProtocol.Items.Insert(i, Dome.protocolOptions[i]);
            }
            
            // select the current protocol if possible
            if (comboBoxProtocol.Items.Contains(Dome.protocol))
            {
                comboBoxProtocol.SelectedItem = Dome.protocol;
            }
           
            // set current server:
            textBoxServer.Text = Dome.server;
           
            // set current port:
            textBoxPort.Text = Dome.port.ToString();
        }
    }
}