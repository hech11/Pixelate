#include "PXpch.h"
#include "PhysicsCollisionCallback.h"

#include "Pixelate/Scene/Entity.h"
#include "Pixelate/Scene/Components.h"

namespace Pixelate {



	void PhysicsCollisionCallback::BeginContact(b2Contact* contact)	{
		CollisionCallBack* callbackA = (CollisionCallBack*)contact->GetFixtureA()->GetBody()->GetUserData();
		CollisionCallBack* callbackB = (CollisionCallBack*)contact->GetFixtureB()->GetBody()->GetUserData();

		Entity contactA = { (entt::entity)callbackA->Handle, callbackA->SceneRef };
		Entity contactB = { (entt::entity)callbackB->Handle, callbackB->SceneRef };

		if (contactA.HasComponent<ScriptingBehaviourComponent>() && ScriptingMaster::ClassExists(contactA.GetComponent<ScriptingBehaviourComponent>().Behaviour.ClassName))
		{
			ScriptingMaster::OnEntityCollisionEnter(contactA.GetComponent<ScriptingBehaviourComponent>().Behaviour, contactB.GetComponent<UUIDComponent>().UUID);
		}

		if (contactB.HasComponent<ScriptingBehaviourComponent>() && ScriptingMaster::ClassExists(contactB.GetComponent<ScriptingBehaviourComponent>().Behaviour.ClassName))
		{
			ScriptingMaster::OnEntityCollisionEnter(contactB.GetComponent<ScriptingBehaviourComponent>().Behaviour, contactA.GetComponent<UUIDComponent>().UUID);
		}
	}

	void PhysicsCollisionCallback::EndContact(b2Contact* contact) {
		CollisionCallBack* callbackA = (CollisionCallBack*)contact->GetFixtureA()->GetBody()->GetUserData();
		CollisionCallBack* callbackB = (CollisionCallBack*)contact->GetFixtureB()->GetBody()->GetUserData();

		Entity contactA = { (entt::entity)callbackA->Handle, callbackA->SceneRef };
		Entity contactB = { (entt::entity)callbackB->Handle, callbackB->SceneRef };

		if (contactA.HasComponent<ScriptingBehaviourComponent>() && ScriptingMaster::ClassExists(contactA.GetComponent<ScriptingBehaviourComponent>().Behaviour.ClassName))
		{
			ScriptingMaster::OnEntityCollisionExit(contactA.GetComponent<ScriptingBehaviourComponent>().Behaviour, &contactB.GetComponent<UUIDComponent>().UUID);
		}

		if (contactB.HasComponent<ScriptingBehaviourComponent>() && ScriptingMaster::ClassExists(contactB.GetComponent<ScriptingBehaviourComponent>().Behaviour.ClassName))
		{
			ScriptingMaster::OnEntityCollisionExit(contactB.GetComponent<ScriptingBehaviourComponent>().Behaviour, &contactA.GetComponent<UUIDComponent>().UUID);
		}
	}

}