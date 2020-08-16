
using System.Runtime.CompilerServices;

namespace Pixelate {

    
    public class RigidBodyComponent {

        public void SetLinearVelocity(Vector2 velocity)
    	{
            SetLinearVelocity_CPP(ref velocity);

        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLinearVelocity_CPP(ref Vector2 vel);
    }
}

