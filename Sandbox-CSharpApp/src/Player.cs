using System;
using Pixelate;



public class Player {

    public Testing test;

    void OnCreate()
    {
        test = new Testing();
        test.Print();
    }


    static void Main(string[] args)
    {

        Player player = new Player();

        player.OnCreate();
    }
    
}
