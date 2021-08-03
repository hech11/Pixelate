using Pixelate;

public class PlayAudio : Entity
{
    public void OnUpdate(float ts)
    {

        if (Input.IsKeyDown(KeyCode.P))
        {
            GetComponent<AudioSourceComponent>().Play();
        }

    }
}
