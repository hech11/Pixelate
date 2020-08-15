using System;

public class Player 
{


    public void OnCreate()
    {
        Console.WriteLine("Player::OnCreate!");
    }
    public void OnUpdate(float ts)
    {
        Console.WriteLine("Player::OnUpdate!: " + ts);
    }

    public void OnDestroy()
    {
        Console.WriteLine("Player::OnDestory!");
    }
}


