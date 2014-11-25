namespace TR_TileEditor
{
    partial class frmTileEditor
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
            this.components = new System.ComponentModel.Container();
            this.pnlTileSet = new System.Windows.Forms.Panel();
            this.pnlTileMap = new System.Windows.Forms.Panel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.overworldMapToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.toolTileSetWidth = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripLabel2 = new System.Windows.Forms.ToolStripLabel();
            this.toolTileSetHeight = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel3 = new System.Windows.Forms.ToolStripLabel();
            this.toolTileWidth = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripLabel4 = new System.Windows.Forms.ToolStripLabel();
            this.toolTileHeight = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel5 = new System.Windows.Forms.ToolStripLabel();
            this.toolTileMapWidth = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripLabel6 = new System.Windows.Forms.ToolStripLabel();
            this.toolTileMapHeight = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.btnLoadTileSet = new System.Windows.Forms.Button();
            this.btnGrid = new System.Windows.Forms.Button();
            this.btnMapLayer = new System.Windows.Forms.Button();
            this.btnEventLayer = new System.Windows.Forms.Button();
            this.btnCollisionLayer = new System.Windows.Forms.Button();
            this.btnPaint = new System.Windows.Forms.Button();
            this.btnErase = new System.Windows.Forms.Button();
            this.btnFill = new System.Windows.Forms.Button();
            this.btnObject = new System.Windows.Forms.Button();
            this.cmbObjects = new System.Windows.Forms.ComboBox();
            this.lblObjects = new System.Windows.Forms.Label();
            this.lstObjects = new System.Windows.Forms.ListBox();
            this.lstWaypoints = new System.Windows.Forms.ListBox();
            this.btnAddWaypoint = new System.Windows.Forms.Button();
            this.btnDeleteWaypoint = new System.Windows.Forms.Button();
            this.btnDeleteObject = new System.Windows.Forms.Button();
            this.numCollisionIDS = new System.Windows.Forms.NumericUpDown();
            this.lblCollisionIDS = new System.Windows.Forms.Label();
            this.btnRemoveEvent = new System.Windows.Forms.Button();
            this.btnAddEvent = new System.Windows.Forms.Button();
            this.tipMap = new System.Windows.Forms.ToolTip(this.components);
            this.btnAddObject = new System.Windows.Forms.Button();
            this.lblObjectsList = new System.Windows.Forms.Label();
            this.lblWaypoints = new System.Windows.Forms.Label();
            this.menuStrip1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numCollisionIDS)).BeginInit();
            this.SuspendLayout();
            // 
            // pnlTileSet
            // 
            this.pnlTileSet.AutoScroll = true;
            this.pnlTileSet.BackColor = System.Drawing.Color.White;
            this.pnlTileSet.Location = new System.Drawing.Point(12, 59);
            this.pnlTileSet.Name = "pnlTileSet";
            this.pnlTileSet.Size = new System.Drawing.Size(347, 263);
            this.pnlTileSet.TabIndex = 0;
            this.pnlTileSet.MouseClick += new System.Windows.Forms.MouseEventHandler(this.pnlTileSet_MouseClick);
            this.pnlTileSet.Resize += new System.EventHandler(this.pnlTileSet_Resize);
            // 
            // pnlTileMap
            // 
            this.pnlTileMap.AutoScroll = true;
            this.pnlTileMap.BackColor = System.Drawing.Color.White;
            this.pnlTileMap.Location = new System.Drawing.Point(365, 59);
            this.pnlTileMap.Name = "pnlTileMap";
            this.pnlTileMap.Size = new System.Drawing.Size(664, 742);
            this.pnlTileMap.TabIndex = 1;
            this.pnlTileMap.MouseClick += new System.Windows.Forms.MouseEventHandler(this.pnlTileMap_MouseClick);
            this.pnlTileMap.MouseEnter += new System.EventHandler(this.pnlTileMap_MouseEnter);
            this.pnlTileMap.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pnlTileMap_MouseClick);
            this.pnlTileMap.Resize += new System.EventHandler(this.pnlTileMap_Resize);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.overworldMapToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1041, 28);
            this.menuStrip1.TabIndex = 8;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.saveMapToolStripMenuItem,
            this.openMapToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(148, 24);
            this.newToolStripMenuItem.Text = "&New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // saveMapToolStripMenuItem
            // 
            this.saveMapToolStripMenuItem.Name = "saveMapToolStripMenuItem";
            this.saveMapToolStripMenuItem.Size = new System.Drawing.Size(148, 24);
            this.saveMapToolStripMenuItem.Text = "&Save Map";
            this.saveMapToolStripMenuItem.Click += new System.EventHandler(this.saveMapToolStripMenuItem_Click);
            // 
            // openMapToolStripMenuItem
            // 
            this.openMapToolStripMenuItem.Name = "openMapToolStripMenuItem";
            this.openMapToolStripMenuItem.Size = new System.Drawing.Size(148, 24);
            this.openMapToolStripMenuItem.Text = "&Open Map";
            this.openMapToolStripMenuItem.Click += new System.EventHandler(this.openMapToolStripMenuItem_Click);
            // 
            // overworldMapToolStripMenuItem
            // 
            this.overworldMapToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showToolStripMenuItem});
            this.overworldMapToolStripMenuItem.Name = "overworldMapToolStripMenuItem";
            this.overworldMapToolStripMenuItem.Size = new System.Drawing.Size(120, 24);
            this.overworldMapToolStripMenuItem.Text = "Overworld&Map";
            // 
            // showToolStripMenuItem
            // 
            this.showToolStripMenuItem.Name = "showToolStripMenuItem";
            this.showToolStripMenuItem.Size = new System.Drawing.Size(175, 24);
            this.showToolStripMenuItem.Text = "&Show";
            this.showToolStripMenuItem.Click += new System.EventHandler(this.showToolStripMenuItem_Click);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripSeparator6,
            this.toolStripSeparator5,
            this.toolStripLabel1,
            this.toolTileSetWidth,
            this.toolStripLabel2,
            this.toolTileSetHeight,
            this.toolStripSeparator2,
            this.toolStripSeparator1,
            this.toolStripLabel3,
            this.toolTileWidth,
            this.toolStripLabel4,
            this.toolTileHeight,
            this.toolStripSeparator4,
            this.toolStripSeparator3,
            this.toolStripLabel5,
            this.toolTileMapWidth,
            this.toolStripLabel6,
            this.toolTileMapHeight,
            this.toolStripSeparator7,
            this.toolStripSeparator8});
            this.toolStrip1.Location = new System.Drawing.Point(0, 28);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(1041, 28);
            this.toolStrip1.TabIndex = 9;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(6, 28);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 28);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(110, 25);
            this.toolStripLabel1.Text = "Tile Set   Width";
            // 
            // toolTileSetWidth
            // 
            this.toolTileSetWidth.AutoSize = false;
            this.toolTileSetWidth.Name = "toolTileSetWidth";
            this.toolTileSetWidth.Size = new System.Drawing.Size(75, 28);
            this.toolTileSetWidth.TextChanged += new System.EventHandler(this.toolTileSetWidth_TextChanged);
            // 
            // toolStripLabel2
            // 
            this.toolStripLabel2.Name = "toolStripLabel2";
            this.toolStripLabel2.Size = new System.Drawing.Size(54, 25);
            this.toolStripLabel2.Text = "Height";
            // 
            // toolTileSetHeight
            // 
            this.toolTileSetHeight.AutoSize = false;
            this.toolTileSetHeight.Name = "toolTileSetHeight";
            this.toolTileSetHeight.Size = new System.Drawing.Size(75, 28);
            this.toolTileSetHeight.TextChanged += new System.EventHandler(this.toolTileSetHeight_TextChanged);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 28);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 28);
            // 
            // toolStripLabel3
            // 
            this.toolStripLabel3.Name = "toolStripLabel3";
            this.toolStripLabel3.Size = new System.Drawing.Size(116, 25);
            this.toolStripLabel3.Text = "Tile Size   Width";
            // 
            // toolTileWidth
            // 
            this.toolTileWidth.AutoSize = false;
            this.toolTileWidth.Name = "toolTileWidth";
            this.toolTileWidth.Size = new System.Drawing.Size(75, 28);
            this.toolTileWidth.TextChanged += new System.EventHandler(this.toolTileWidth_TextChanged);
            // 
            // toolStripLabel4
            // 
            this.toolStripLabel4.Name = "toolStripLabel4";
            this.toolStripLabel4.Size = new System.Drawing.Size(54, 25);
            this.toolStripLabel4.Text = "Height";
            // 
            // toolTileHeight
            // 
            this.toolTileHeight.AutoSize = false;
            this.toolTileHeight.Name = "toolTileHeight";
            this.toolTileHeight.Size = new System.Drawing.Size(75, 28);
            this.toolTileHeight.TextChanged += new System.EventHandler(this.toolTileHeight_TextChanged);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 28);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 28);
            // 
            // toolStripLabel5
            // 
            this.toolStripLabel5.Name = "toolStripLabel5";
            this.toolStripLabel5.Size = new System.Drawing.Size(122, 25);
            this.toolStripLabel5.Text = "Map Size   Width";
            // 
            // toolTileMapWidth
            // 
            this.toolTileMapWidth.AutoSize = false;
            this.toolTileMapWidth.Name = "toolTileMapWidth";
            this.toolTileMapWidth.Size = new System.Drawing.Size(75, 28);
            this.toolTileMapWidth.TextChanged += new System.EventHandler(this.toolTileMapWidth_TextChanged);
            // 
            // toolStripLabel6
            // 
            this.toolStripLabel6.Name = "toolStripLabel6";
            this.toolStripLabel6.Size = new System.Drawing.Size(54, 25);
            this.toolStripLabel6.Text = "Height";
            // 
            // toolTileMapHeight
            // 
            this.toolTileMapHeight.AutoSize = false;
            this.toolTileMapHeight.Name = "toolTileMapHeight";
            this.toolTileMapHeight.Size = new System.Drawing.Size(75, 28);
            this.toolTileMapHeight.TextChanged += new System.EventHandler(this.toolTileMapHeight_TextChanged);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 28);
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(6, 28);
            // 
            // btnLoadTileSet
            // 
            this.btnLoadTileSet.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnLoadTileSet.Location = new System.Drawing.Point(13, 328);
            this.btnLoadTileSet.Name = "btnLoadTileSet";
            this.btnLoadTileSet.Size = new System.Drawing.Size(136, 33);
            this.btnLoadTileSet.TabIndex = 10;
            this.btnLoadTileSet.Text = "Load Tile Set";
            this.btnLoadTileSet.UseVisualStyleBackColor = false;
            this.btnLoadTileSet.Click += new System.EventHandler(this.btnLoadTileSet_Click);
            // 
            // btnGrid
            // 
            this.btnGrid.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnGrid.Location = new System.Drawing.Point(224, 328);
            this.btnGrid.Name = "btnGrid";
            this.btnGrid.Size = new System.Drawing.Size(136, 33);
            this.btnGrid.TabIndex = 11;
            this.btnGrid.Text = "Toggle Grid";
            this.btnGrid.UseVisualStyleBackColor = false;
            this.btnGrid.Click += new System.EventHandler(this.btnGrid_Click);
            // 
            // btnMapLayer
            // 
            this.btnMapLayer.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnMapLayer.Location = new System.Drawing.Point(12, 367);
            this.btnMapLayer.Name = "btnMapLayer";
            this.btnMapLayer.Size = new System.Drawing.Size(101, 47);
            this.btnMapLayer.TabIndex = 12;
            this.btnMapLayer.Text = "Map\r\nLayer";
            this.btnMapLayer.UseVisualStyleBackColor = false;
            this.btnMapLayer.Click += new System.EventHandler(this.btnMapLayer_Click);
            // 
            // btnEventLayer
            // 
            this.btnEventLayer.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnEventLayer.Location = new System.Drawing.Point(258, 367);
            this.btnEventLayer.Name = "btnEventLayer";
            this.btnEventLayer.Size = new System.Drawing.Size(101, 47);
            this.btnEventLayer.TabIndex = 13;
            this.btnEventLayer.Text = "Event\r\nLayer";
            this.btnEventLayer.UseVisualStyleBackColor = false;
            this.btnEventLayer.Click += new System.EventHandler(this.btnEventLayer_Click);
            // 
            // btnCollisionLayer
            // 
            this.btnCollisionLayer.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnCollisionLayer.Location = new System.Drawing.Point(135, 367);
            this.btnCollisionLayer.Name = "btnCollisionLayer";
            this.btnCollisionLayer.Size = new System.Drawing.Size(101, 47);
            this.btnCollisionLayer.TabIndex = 14;
            this.btnCollisionLayer.Text = "Collision Layer";
            this.btnCollisionLayer.UseVisualStyleBackColor = false;
            this.btnCollisionLayer.Click += new System.EventHandler(this.btnCollisionLayer_Click);
            // 
            // btnPaint
            // 
            this.btnPaint.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnPaint.Location = new System.Drawing.Point(17, 420);
            this.btnPaint.Name = "btnPaint";
            this.btnPaint.Size = new System.Drawing.Size(90, 30);
            this.btnPaint.TabIndex = 15;
            this.btnPaint.Text = "Paint";
            this.btnPaint.UseVisualStyleBackColor = false;
            this.btnPaint.Click += new System.EventHandler(this.btnPaint_Click);
            // 
            // btnErase
            // 
            this.btnErase.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnErase.Location = new System.Drawing.Point(17, 492);
            this.btnErase.Name = "btnErase";
            this.btnErase.Size = new System.Drawing.Size(90, 30);
            this.btnErase.TabIndex = 16;
            this.btnErase.Text = "Erase";
            this.btnErase.UseVisualStyleBackColor = false;
            this.btnErase.Click += new System.EventHandler(this.btnErase_Click);
            // 
            // btnFill
            // 
            this.btnFill.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnFill.Location = new System.Drawing.Point(17, 456);
            this.btnFill.Name = "btnFill";
            this.btnFill.Size = new System.Drawing.Size(90, 30);
            this.btnFill.TabIndex = 17;
            this.btnFill.Text = "Fill";
            this.btnFill.UseVisualStyleBackColor = false;
            this.btnFill.Click += new System.EventHandler(this.btnFill_Click);
            // 
            // btnObject
            // 
            this.btnObject.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnObject.Location = new System.Drawing.Point(55, 543);
            this.btnObject.Name = "btnObject";
            this.btnObject.Size = new System.Drawing.Size(261, 34);
            this.btnObject.TabIndex = 18;
            this.btnObject.Text = "Object Layer";
            this.btnObject.UseVisualStyleBackColor = false;
            this.btnObject.Click += new System.EventHandler(this.btnObject_Click);
            // 
            // cmbObjects
            // 
            this.cmbObjects.Enabled = false;
            this.cmbObjects.FormattingEnabled = true;
            this.cmbObjects.Items.AddRange(new object[] {
            "Player",
            "Guard"});
            this.cmbObjects.Location = new System.Drawing.Point(194, 583);
            this.cmbObjects.Name = "cmbObjects";
            this.cmbObjects.Size = new System.Drawing.Size(115, 24);
            this.cmbObjects.TabIndex = 19;
            // 
            // lblObjects
            // 
            this.lblObjects.AutoSize = true;
            this.lblObjects.Location = new System.Drawing.Point(10, 586);
            this.lblObjects.Name = "lblObjects";
            this.lblObjects.Size = new System.Drawing.Size(139, 17);
            this.lblObjects.TabIndex = 20;
            this.lblObjects.Text = "Game World Objects";
            // 
            // lstObjects
            // 
            this.lstObjects.FormattingEnabled = true;
            this.lstObjects.ItemHeight = 16;
            this.lstObjects.Location = new System.Drawing.Point(12, 739);
            this.lstObjects.Name = "lstObjects";
            this.lstObjects.Size = new System.Drawing.Size(135, 148);
            this.lstObjects.TabIndex = 21;
            this.lstObjects.MouseClick += new System.Windows.Forms.MouseEventHandler(this.lstObjects_MouseClick);
            // 
            // lstWaypoints
            // 
            this.lstWaypoints.FormattingEnabled = true;
            this.lstWaypoints.ItemHeight = 16;
            this.lstWaypoints.Location = new System.Drawing.Point(223, 739);
            this.lstWaypoints.Name = "lstWaypoints";
            this.lstWaypoints.Size = new System.Drawing.Size(136, 148);
            this.lstWaypoints.TabIndex = 22;
            // 
            // btnAddWaypoint
            // 
            this.btnAddWaypoint.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnAddWaypoint.Enabled = false;
            this.btnAddWaypoint.Location = new System.Drawing.Point(251, 616);
            this.btnAddWaypoint.Name = "btnAddWaypoint";
            this.btnAddWaypoint.Size = new System.Drawing.Size(102, 43);
            this.btnAddWaypoint.TabIndex = 23;
            this.btnAddWaypoint.Text = "Add Waypoint";
            this.btnAddWaypoint.UseVisualStyleBackColor = false;
            this.btnAddWaypoint.Click += new System.EventHandler(this.btnAddWaypoint_Click);
            // 
            // btnDeleteWaypoint
            // 
            this.btnDeleteWaypoint.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnDeleteWaypoint.Enabled = false;
            this.btnDeleteWaypoint.Location = new System.Drawing.Point(251, 665);
            this.btnDeleteWaypoint.Name = "btnDeleteWaypoint";
            this.btnDeleteWaypoint.Size = new System.Drawing.Size(102, 43);
            this.btnDeleteWaypoint.TabIndex = 24;
            this.btnDeleteWaypoint.Text = "Delete Waypoint";
            this.btnDeleteWaypoint.UseVisualStyleBackColor = false;
            this.btnDeleteWaypoint.Click += new System.EventHandler(this.btnDeleteWaypoint_Click);
            // 
            // btnDeleteObject
            // 
            this.btnDeleteObject.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnDeleteObject.Enabled = false;
            this.btnDeleteObject.Location = new System.Drawing.Point(17, 665);
            this.btnDeleteObject.Name = "btnDeleteObject";
            this.btnDeleteObject.Size = new System.Drawing.Size(102, 43);
            this.btnDeleteObject.TabIndex = 25;
            this.btnDeleteObject.Text = "Delete Object";
            this.btnDeleteObject.UseVisualStyleBackColor = false;
            this.btnDeleteObject.Click += new System.EventHandler(this.btnDeleteObject_Click);
            // 
            // numCollisionIDS
            // 
            this.numCollisionIDS.Enabled = false;
            this.numCollisionIDS.Location = new System.Drawing.Point(140, 497);
            this.numCollisionIDS.Maximum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numCollisionIDS.Name = "numCollisionIDS";
            this.numCollisionIDS.Size = new System.Drawing.Size(90, 22);
            this.numCollisionIDS.TabIndex = 26;
            // 
            // lblCollisionIDS
            // 
            this.lblCollisionIDS.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCollisionIDS.Location = new System.Drawing.Point(135, 427);
            this.lblCollisionIDS.Name = "lblCollisionIDS";
            this.lblCollisionIDS.Size = new System.Drawing.Size(102, 52);
            this.lblCollisionIDS.TabIndex = 27;
            this.lblCollisionIDS.Text = "0 = No Collision\r\n1 = Wall\r\n2 = Muck";
            // 
            // btnRemoveEvent
            // 
            this.btnRemoveEvent.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnRemoveEvent.Enabled = false;
            this.btnRemoveEvent.Location = new System.Drawing.Point(263, 476);
            this.btnRemoveEvent.Name = "btnRemoveEvent";
            this.btnRemoveEvent.Size = new System.Drawing.Size(90, 46);
            this.btnRemoveEvent.TabIndex = 28;
            this.btnRemoveEvent.Text = "Remove Event";
            this.btnRemoveEvent.UseVisualStyleBackColor = false;
            this.btnRemoveEvent.Click += new System.EventHandler(this.btnRemoveEvent_Click);
            // 
            // btnAddEvent
            // 
            this.btnAddEvent.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnAddEvent.Enabled = false;
            this.btnAddEvent.Location = new System.Drawing.Point(263, 420);
            this.btnAddEvent.Name = "btnAddEvent";
            this.btnAddEvent.Size = new System.Drawing.Size(90, 46);
            this.btnAddEvent.TabIndex = 29;
            this.btnAddEvent.Text = "Add\r\nEvent";
            this.btnAddEvent.UseVisualStyleBackColor = false;
            this.btnAddEvent.Click += new System.EventHandler(this.btnAddEvent_Click);
            // 
            // btnAddObject
            // 
            this.btnAddObject.BackColor = System.Drawing.SystemColors.ControlLight;
            this.btnAddObject.Enabled = false;
            this.btnAddObject.Location = new System.Drawing.Point(17, 616);
            this.btnAddObject.Name = "btnAddObject";
            this.btnAddObject.Size = new System.Drawing.Size(102, 43);
            this.btnAddObject.TabIndex = 30;
            this.btnAddObject.Text = "Add Object";
            this.btnAddObject.UseVisualStyleBackColor = false;
            this.btnAddObject.Click += new System.EventHandler(this.btnAddObject_Click);
            // 
            // lblObjectsList
            // 
            this.lblObjectsList.AutoSize = true;
            this.lblObjectsList.Location = new System.Drawing.Point(10, 719);
            this.lblObjectsList.Name = "lblObjectsList";
            this.lblObjectsList.Size = new System.Drawing.Size(120, 17);
            this.lblObjectsList.TabIndex = 31;
            this.lblObjectsList.Text = "Level Objects List";
            // 
            // lblWaypoints
            // 
            this.lblWaypoints.AutoSize = true;
            this.lblWaypoints.Location = new System.Drawing.Point(221, 719);
            this.lblWaypoints.Name = "lblWaypoints";
            this.lblWaypoints.Size = new System.Drawing.Size(93, 17);
            this.lblWaypoints.TabIndex = 32;
            this.lblWaypoints.Text = "Waypoint List";
            // 
            // frmTileEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1041, 899);
            this.Controls.Add(this.lblWaypoints);
            this.Controls.Add(this.lblObjectsList);
            this.Controls.Add(this.btnAddObject);
            this.Controls.Add(this.btnDeleteObject);
            this.Controls.Add(this.btnAddEvent);
            this.Controls.Add(this.btnRemoveEvent);
            this.Controls.Add(this.lblCollisionIDS);
            this.Controls.Add(this.numCollisionIDS);
            this.Controls.Add(this.btnDeleteWaypoint);
            this.Controls.Add(this.btnAddWaypoint);
            this.Controls.Add(this.lstWaypoints);
            this.Controls.Add(this.lstObjects);
            this.Controls.Add(this.lblObjects);
            this.Controls.Add(this.cmbObjects);
            this.Controls.Add(this.btnObject);
            this.Controls.Add(this.btnFill);
            this.Controls.Add(this.btnErase);
            this.Controls.Add(this.btnPaint);
            this.Controls.Add(this.btnCollisionLayer);
            this.Controls.Add(this.btnEventLayer);
            this.Controls.Add(this.btnMapLayer);
            this.Controls.Add(this.btnGrid);
            this.Controls.Add(this.btnLoadTileSet);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.pnlTileMap);
            this.Controls.Add(this.pnlTileSet);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Location = new System.Drawing.Point(75, 75);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "frmTileEditor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Trapped Rat Tile Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numCollisionIDS)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel pnlTileSet;
        private System.Windows.Forms.Panel pnlTileMap;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem overworldMapToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripComboBox toolTileSetWidth;
        private System.Windows.Forms.ToolStripLabel toolStripLabel2;
        private System.Windows.Forms.ToolStripComboBox toolTileSetHeight;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel3;
        private System.Windows.Forms.ToolStripComboBox toolTileWidth;
        private System.Windows.Forms.ToolStripLabel toolStripLabel4;
        private System.Windows.Forms.ToolStripComboBox toolTileHeight;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripLabel toolStripLabel5;
        private System.Windows.Forms.ToolStripComboBox toolTileMapWidth;
        private System.Windows.Forms.ToolStripLabel toolStripLabel6;
        private System.Windows.Forms.ToolStripComboBox toolTileMapHeight;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.Button btnLoadTileSet;
        private System.Windows.Forms.Button btnGrid;
        private System.Windows.Forms.Button btnMapLayer;
        private System.Windows.Forms.Button btnEventLayer;
        private System.Windows.Forms.Button btnCollisionLayer;
        private System.Windows.Forms.Button btnPaint;
        private System.Windows.Forms.Button btnErase;
        private System.Windows.Forms.Button btnFill;
        private System.Windows.Forms.ToolStripMenuItem saveMapToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openMapToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.Button btnObject;
        private System.Windows.Forms.ComboBox cmbObjects;
        private System.Windows.Forms.Label lblObjects;
        private System.Windows.Forms.ListBox lstObjects;
        private System.Windows.Forms.ListBox lstWaypoints;
        private System.Windows.Forms.Button btnAddWaypoint;
        private System.Windows.Forms.Button btnDeleteWaypoint;
        private System.Windows.Forms.Button btnDeleteObject;
        private System.Windows.Forms.NumericUpDown numCollisionIDS;
        private System.Windows.Forms.Label lblCollisionIDS;
        private System.Windows.Forms.Button btnRemoveEvent;
        private System.Windows.Forms.Button btnAddEvent;
        private System.Windows.Forms.ToolTip tipMap;
        private System.Windows.Forms.Button btnAddObject;
        private System.Windows.Forms.Label lblObjectsList;
        private System.Windows.Forms.Label lblWaypoints;
    }
}

