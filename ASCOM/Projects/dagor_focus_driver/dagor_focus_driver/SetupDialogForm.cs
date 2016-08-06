using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using ASCOM.Utilities;
using ASCOM.DagorFocus;

namespace ASCOM.DagorFocus
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
            Focuser.protocol = (string)comboBoxProtocol.SelectedItem;
            Focuser.server = (string)textBoxServer.Text;
            Focuser.port = int.Parse(textBoxPort.Text);
            Focuser.tl.Enabled = chkTrace.Checked;
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
            chkTrace.Checked = Focuser.tl.Enabled;
            // set the list of com ports to those that are currently available
            comboBoxProtocol.Items.Clear();
            for (int i=0; i < Focuser.protocolOptions.Count; i++)
            {
                comboBoxProtocol.Items.Insert(i, Focuser.protocolOptions[i]);
            }
            // select the current protocol if possible
            if (comboBoxProtocol.Items.Contains(Focuser.protocol))
            {
                comboBoxProtocol.SelectedItem = Focuser.protocol;
            }
            // set current server:
            textBoxServer.Text = Focuser.server;
            // set current port:
            textBoxPort.Text = Focuser.port.ToString();
        }
    }
}