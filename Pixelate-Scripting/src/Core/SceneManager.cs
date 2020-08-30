using System;
using System.Runtime.CompilerServices;

namespace Pixelate
{

    public class SceneManager {

        public static void LoadScene(String filepath) {
            LoadScene_CPP(filepath);
        }



        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LoadScene_CPP(String filepath);

    }

}