using System;
using System.Runtime.CompilerServices;
using System.Runtime.Remoting.Messaging;

namespace Pixelate { 

    public class Entity 
    {

        public ulong UUID { 
            get; private set; 
        }

        ~Entity()
        {

        }




        public T GetComponent<T>() where T : Component, new()
        {
            T comp = new T();
            comp.Entity = this;
            return comp;
        }


        public Matrix4x4 GetTransform()
        {
            Matrix4x4 temp;
            GetTransform_CPP(UUID, out temp);
            return temp;
        }

        public void SetTransform(Matrix4x4 transform)
        {
            SetTransform_CPP(UUID, ref transform);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetTransform_CPP(ulong entity, out Matrix4x4 matrix);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTransform_CPP(ulong entity, ref Matrix4x4 matrix);


    }
}


