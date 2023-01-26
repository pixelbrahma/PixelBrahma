using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using PixelBrahma;

namespace Sandbox
{
	// The Bird class
	public class CameraController : Entity
	{
		private TransformComponent m_Transform;

		float speed = 1f;
		Vector3 m_Velocity = Vector3.Zero;

		// Create player callback
		void OnCreate()
		{
			Console.WriteLine($"Player.OnCreate - {ID}");

			m_Transform = GetComponent<TransformComponent>();
			m_Velocity.X = 1f;
		}

		// Player update function
		void OnUpdate(float timestep)
		{
			// Console.WriteLine($"Player.OnUpdate: {timestep}");

			m_Velocity.Y = 0f;

			m_Velocity.X *= speed;

			Vector3 translation = m_Transform.Translation;
			translation += m_Velocity * timestep;
			m_Transform.Translation = translation;
		}
	}
}
