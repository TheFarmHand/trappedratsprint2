using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TR_TileEditor
{
    public struct Tile
    {
        public Tile(int TID, int CID)
        {
            tileID = TID;
            collisionID = CID;
            eventID = "none";
            onFireID = "none";
            objectID = '0';
            objectNumber = 0;
        }
        int tileID;
        public int TileID
        {
            get { return tileID; }
            set { tileID = value; }
        }
        int collisionID;
        public int CollisionID
        {
            get { return collisionID; }
            set { collisionID = value; }
        }
        string eventID;
        public string EventID
        {
            get { return eventID; }
            set { eventID = value; }
        }
        string onFireID;
        public string OnFireID
        {
            get { return onFireID; }
            set { onFireID = value; }
        }
        char objectID;
        public char ObjectID
        {
            get { return objectID; }
            set { objectID = value; }
        }
        int objectNumber;
        public int ObjectNumber
        {
            get { return objectNumber; }
            set { objectNumber = value; }
        }
    }
}
