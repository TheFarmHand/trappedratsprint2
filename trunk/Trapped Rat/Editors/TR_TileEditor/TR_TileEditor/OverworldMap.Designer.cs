namespace TR_TileEditor
{
    partial class OverworldMap
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
            this.pnlOverMap = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // pnlOverMap
            // 
            this.pnlOverMap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlOverMap.Location = new System.Drawing.Point(0, 0);
            this.pnlOverMap.Name = "pnlOverMap";
            this.pnlOverMap.Size = new System.Drawing.Size(732, 813);
            this.pnlOverMap.TabIndex = 0;
            this.pnlOverMap.MouseClick += new System.Windows.Forms.MouseEventHandler(this.pnlOverMap_MouseClick);
            // 
            // OverworldMap
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(732, 813);
            this.Controls.Add(this.pnlOverMap);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "OverworldMap";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Overworld Map";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pnlOverMap;
    }
}