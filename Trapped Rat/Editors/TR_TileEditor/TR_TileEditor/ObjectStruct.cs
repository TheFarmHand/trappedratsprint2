using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TR_TileEditor
{
    public struct ObjectStruct
    {
        string name;
        public string Name
        {
            get { return name; }
            set { name = value; }
        }
        List<int> waypointTiles;
        public List<int> WaypointTiles
        {
            get { return waypointTiles; }
            set { waypointTiles = value; }
        }
        public ObjectStruct(string _name)
        {
            name = _name;
            waypointTiles = new List<int>();
        }

    }
}
