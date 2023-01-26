using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using PixelBrahma;

namespace Sandbox
{
	// The Paddle class of the Right side player
	public class PaddleRight : Entity
	{
		private TransformComponent m_Transform;

		float speed = 30f;
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

			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeyDown(KeyCode.Up))
				velocity.Y = 1;
			else if (Input.IsKeyDown(KeyCode.Down))
				velocity.Y = -1;
			else
				velocity.Y = 0;

			velocity *= speed;

			Vector3 translation = m_Transform.Translation;
			translation += velocity * timestep;
			m_Transform.Translation = translation;

		}
	}
}
