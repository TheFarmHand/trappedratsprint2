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


namespace Character_Editor
{
    public partial class CharacterEditor : Form
    {
        public enum Sides {Ally, Enemy};
        public enum elements { Wind, Water, Earth, Fire, Multi };
        String _enemy = "Ally";
        public CharacterEditor()
        {
            InitializeComponent();
            Side.DataSource = Enum.GetValues(typeof(Sides));
            Element.DataSource = Enum.GetValues(typeof(elements));
        }

        
        

        private void Side_TextChanged(object sender, EventArgs e)
        {
            if(Side.Text == "Enemy")
            {
                Element.Visible = true;
                label10.Visible = true;
                radioButton1.Visible = true;
                
                radioButton3.Visible = true;
                radioButton4.Visible = true;
            }
            else
            {
                Element.Visible = false;
                label10.Visible = false;
                radioButton1.Visible = false;
                
                radioButton3.Visible = false;
                radioButton4.Visible = false;
            }
        }

        

        private void AttackBrowse_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Sound |*.wav";
            if(dlg.ShowDialog() == DialogResult.OK)
            {
                for (int i = dlg.FileName.Length - 1; i >= 0; i--)
                {
                    if (dlg.FileName[i] == '\\' || dlg.FileName[i] == '/')
                    {
                        for (int j = i + 1; j < dlg.FileName.Length; j++)
                        {
                            AttackSound.Text += dlg.FileName[j];
                        }
                        break;
                    }
                }
                
            }
        }

        private void DeathBrowse_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Sound |*.wav";
            if(dlg.ShowDialog() == DialogResult.OK)
            {
                for (int i = dlg.FileName.Length - 1; i >= 0; i--)
                {
                    if (dlg.FileName[i] == '\\' || dlg.FileName[i] == '/')
                    {
                        for (int j = i + 1; j < dlg.FileName.Length; j++)
                        {
                            DeathSound.Text += dlg.FileName[j];
                        }
                        break;
                    }
                }
                
            }
        }

       

        private void XMLButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "XML | *.xml";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                for (int i = dlg.FileName.Length - 1; i >= 0; i--)
                {
                    if (dlg.FileName[i] == '\\' || dlg.FileName[i] == '/')
                    {
                        for (int j = i + 1; j < dlg.FileName.Length; j++)
                        {
                            XMLbox.Text += dlg.FileName[j];

                        }
                        break;
                    }
                }
            }
        }

        private void HurtBrowse_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Sound |*.wav";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                for (int i = dlg.FileName.Length - 1; i >= 0; i--)
                {
                    if (dlg.FileName[i] == '\\' || dlg.FileName[i] == '/')
                    {
                        for (int j = i + 1; j < dlg.FileName.Length; j++)
                        {
                           HurtSound.Text += dlg.FileName[j];
                        }
                        break;
                    }
                }
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(radioButton3.Checked)
            {
                _enemy = "Boss";
            }
            else if(radioButton1.Checked)
            {
                _enemy = "Guard";
            }
            else if(radioButton4.Checked)
            {
                _enemy = "Enemy";
            }
            SaveFileDialog dlg = new SaveFileDialog();
            XDocument xdoc = new XDocument();
            if(dlg.ShowDialog() == DialogResult.OK)
            {
                string randomstring = "false";
                if(Random.Checked)
                {
                    randomstring = "true";
                }
                xdoc.Add(new XElement("Character",
                                new XElement("Name", name.Text),
                                new XElement("Side", Side.Text),
                                new XElement("Element", Element.Text),
                                new XElement("Type", _enemy),
                                new XElement("IsRandom",randomstring),
                                new XElement("Stats",
                                        new XElement("HitPoints",
                                            new XElement("Base", HitPoints.Value),
                                            new XElement("Scale", HPScale.Value)),
                                        new XElement("BattlePoints",
                                                new XElement("Base", BattlePoints.Value),
                                                new XElement("Scale", BPScale.Value)),
                                        new XElement("Attack",
                                            new XElement("Base", Attack.Value),
                                            new XElement("Scale", AttackScale.Value)),
                                        new XElement("Magic",
                                            new XElement("Base", Magic.Value),
                                            new XElement("Scale", MagicScale.Value)),
                                        new XElement("Defense",
                                            new XElement("Base", Defense.Value),
                                            new XElement("Scale", DefenseScale.Value)),
                                        new XElement("Speed",
                                            new XElement("Base", Speed.Value),
                                            new XElement("Scale", SpeedScale.Value)),
                                        new XElement("Avoision",
                                            new XElement("Base", Avoision.Value),
                                            new XElement("Scale", AvoisionScale.Value))),
                                new XElement("Level", numericUpDown1.Value),
                                new XElement("Sound",
                                        new XElement("Hurt",HurtSound.Text),
                                        new XElement("Attack",AttackSound.Text),
                                        new XElement("Death", DeathSound.Text)),
                                new XElement("Animation",XMLbox.Text)
                                        ));
                


                xdoc.Save(dlg.FileName);
            }
        }

       

        

        

       

       

       

       

        
    }
}
