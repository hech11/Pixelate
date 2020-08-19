using System.Runtime.CompilerServices;

namespace Pixelate {


    public abstract class Component {
        public Entity Entity { get; set; }
    }
    
    public class RigidBodyComponent : Component {

        public void SetLinearVelocity(Vector2 velocity)
    	{
            SetLinearVelocity_CPP(Entity.UUID, ref velocity);

        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLinearVelocity_CPP(ulong entity, ref Vector2 vel);
    }


    // just to test audio
    public class AudioSourceComponent : Component {
        public void Play()
        {
            Play_CPP(Entity.UUID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Play_CPP(ulong entity);

    }

}

