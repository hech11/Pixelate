using System.Runtime.InteropServices;

namespace Pixelate
{

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2
    {
        public float x, y;


        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;

        }


    }



}


