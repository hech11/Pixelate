using System;

namespace Pixelate
{

    public class Entity {
        public int Health = 5;

        ~Entity()
        {

        }

        public void PrintHealth() {
            Console.WriteLine(Health);
        }

    }


}