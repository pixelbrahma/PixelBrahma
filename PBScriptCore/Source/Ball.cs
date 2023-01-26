using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using PixelBrahma;

namespace Sandbox
{
	// The Ball class
	public class Ball : Entity
	{
		private TransformComponent m_Transform;

		float speed = 5f;
		Vector3 velocity = Vector3.Zero;

		// Create Ball callback
		void OnCreate()
		{
			Console.WriteLine($"Player.OnCreate - {ID}");

			m_Transform = GetComponent<TransformComponent>();
		}

		// Ball update function
		void OnUpdate(float timestep)
		{
			// Console.WriteLine($"Player.OnUpdate: {timestep}");

			if (Input.IsKeyDown(KeyCode.Space))
				Start();

			Vector3 translation = m_Transform.Translation;
			translation += velocity * timestep;
			m_Transform.Translation = translation;
		}

		// Start the game
		void Start()
		{
			Random random = new Random();
			int rand = random.Next();

			if (rand % 2 == 0)
				velocity.X = 1;
			else
				velocity.X = -1;

			rand = random.Next();

			if (rand % 2 == 0)
				velocity.Y = 0.5f;
			else
				velocity.Y = -0.5f;

			velocity *= speed;
		}

	}
}
