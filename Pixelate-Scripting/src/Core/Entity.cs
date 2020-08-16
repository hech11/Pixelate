using System;
using System.Runtime.CompilerServices;
using System.Runtime.Remoting.Messaging;

namespace Pixelate { 

    public class Entity 
    {
        ~Entity()
        {

        }

        // Not sure if this is safe
        public T GetComponent<T>() where T : new()
        {
            T comp = new T();
            return comp;
        }


        public Matrix4x4 GetTransform()
        {
            Matrix4x4 temp;
            GetTransform_CPP(out temp);
            return temp;
        }

        public void SetTransform(Matrix4x4 transform)
        {
            SetTransform_CPP(ref transform);
        }


        // assuming there is one script in the scene. TODO: Add entity UUID's
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetTransform_CPP(out Matrix4x4 matrix);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTransform_CPP(ref Matrix4x4 matrix);

    }
}


