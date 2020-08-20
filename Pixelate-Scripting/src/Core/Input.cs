using System.Runtime.CompilerServices;

namespace Pixelate
{

    public class Input
    {

        public static bool IsKeyDown(KeyCode code) {
            return IsKeyDown_CPP(out code);
        }

        public static bool IsMouseButtonDown(MouseButton code)
        {
            return IsMouseButtonDown_CPP(out code);
        }
        public static Vector2 GetMousePosition()
        {
            Vector2 temp = new Vector2();
            GetMousePosition_CPP(out temp);
            return temp;
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsKeyDown_CPP(out KeyCode code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsMouseButtonDown_CPP(out MouseButton code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetMousePosition_CPP(out Vector2 position);
    }

}



