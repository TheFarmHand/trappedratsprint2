using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;
using System.Xml;
using System.IO;
namespace DialogueEditor
{
    
    public partial class Form1 : Form
    {
        bool selected = false;
        string relativepath = "";
        public struct message
        {
            public string words;//will hold what the message says
            public string portrait;//will hold a portait of who is saying things
            public string name;//name of the specific message...this is only for the editor

            public override string ToString()
            {
                if (name == "")
                    return "no name";
                else
                    return name;
            }

        }

       // public List<message> messages;
        public Form1()
        {
            InitializeComponent();
           // MessagesBox.DataSource = messages;
            MessageBox.Show("Find The Scripts Directory", "Directory", MessageBoxButtons.OKCancel);
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if(dlg.ShowDialog() == DialogResult.OK)
            {
               relativepath = System.IO.Path.GetFullPath(dlg.SelectedPath);
            }
            
        }

        private void SaveButton_MouseClick(object sender, MouseEventArgs e)
        {
            if (NameBox.Text == "" || WordsBox.Text == "" || PortraitBox.Text == "")
                return;
            message temp = new message();
            temp.name = NameBox.Text;
            temp.words = WordsBox.Text;
            temp.portrait = PortraitBox.Text;

           // messages.Add(temp);
            if (selected)
                MessagesBox.Items[MessagesBox.SelectedIndex] = temp;
            else
                MessagesBox.Items.Add(temp);

            NameBox.Clear();
            WordsBox.Clear();
            PortraitBox.Clear();
            MessagesBox.ClearSelected();
            selected = false;

        }

        private void UpButton_MouseClick(object sender, MouseEventArgs e)
        {
            if (!selected || MessagesBox.SelectedIndex - 1 < 0)
                return;


            int index1 = MessagesBox.SelectedIndex;
            int index2 = index1 - 1;
            message temp1 = (message)MessagesBox.Items[index1];
            message temp2 = (message)MessagesBox.Items[index2];

            MessagesBox.Items[index2] = temp1;
            MessagesBox.Items[index1] = temp2;
            MessagesBox.SelectedIndex -= 1;

        }

        private void DownButton_MouseClick(object sender, MouseEventArgs e)
        {
            if (!selected || MessagesBox.SelectedIndex + 2 > MessagesBox.Items.Count)
                return;


            int  index1 = MessagesBox.SelectedIndex;
            int index2 = index1 + 1;
            message temp1 = (message)MessagesBox.Items[index1];
            message temp2 = (message)MessagesBox.Items[index2];

            MessagesBox.Items[index2] = temp1;
            MessagesBox.Items[index1] = temp2;
            MessagesBox.SelectedIndex += 1;

        }

        private void DeselectButton_MouseClick(object sender, MouseEventArgs e)
        {
            selected = false;
            MessagesBox.ClearSelected();
            NameBox.Clear();
            WordsBox.Clear();
            PortraitBox.Clear();
        }

        private void MessagesBox_SelectedValueChanged(object sender, EventArgs e)
        {
            
            if (MessagesBox.SelectedItem == null)
                return;

            message temp;
            temp = (message)MessagesBox.SelectedItem;
            NameBox.Text = temp.name;
            PortraitBox.Text = temp.portrait;
            WordsBox.Text = temp.words;
            selected = true;
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.InitialDirectory = relativepath;
            if(DialogResult.OK == dlg.ShowDialog())
            {

                XDocument doc = new XDocument();
                XElement dialogue = new XElement("Dialogue");
                        
                for (int i = 0; i < MessagesBox.Items.Count;i++ )
                {
                    message temp = (message)MessagesBox.Items[i];
                    XElement Message = new XElement("Message",
                                 new XElement("Name", temp.name),
                                 new XElement("Portrait", temp.portrait),
                                 new XElement("Words", temp.words));
                    dialogue.Add(Message);

                }

                doc.Add(dialogue);
                doc.Save(dlg.FileName);
               
            }
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NameBox.Text = "";
            PortraitBox.Text = "";
            WordsBox.Text = "";
            MessagesBox.Items.Clear();
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.InitialDirectory = relativepath;
            if (DialogResult.OK == dlg.ShowDialog())
            {
                XElement doc = XElement.Load(dlg.FileName);
                IEnumerable<XElement> Messages = doc.Elements();
                foreach(XElement Message in Messages)
                {
                    message temp = new message();


                   temp.name =  Message.Element("Name").Value;
                   temp.portrait = Message.Element("Portrait").Value;
                    temp.words = Message.Element("Words").Value;

                    MessagesBox.Items.Add(temp);
                }
              

                


            }
        }

        private void DeleteButton_Click(object sender, EventArgs e)
        {
            int index = MessagesBox.SelectedIndex;
            if (!(index < 0))
            {
                MessagesBox.Items.RemoveAt(index);
                MessagesBox.ClearSelected();
                NameBox.Text = "";
                PortraitBox.Text = "";
                WordsBox.Text = "";
            }
        }

        private void BrowseButton_Click(object sender, EventArgs e)
        {
            PortraitBox.Text = "";
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.InitialDirectory = relativepath;
            if(DialogResult.OK == dlg.ShowDialog())
            {
                
                for (int i = dlg.FileName.Length - 1; i >= 0;i-- )
                {
                    
                    if(dlg.FileName[i] == '\\')
                    {
                        for(int j = i + 1;j < dlg.FileName.Length;j++ )
                        {
                            PortraitBox.Text += dlg.FileName[j];
                        }
                        break;
                    }
                }
                   
            }
        }

        

        

        

        
    }
    
}
