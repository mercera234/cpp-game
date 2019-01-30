#include "CppUnitTest.h"
#include "ResourceManager.h"
#include "Actor.h"
#include "actor_helper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(ActorTest)
	{
		ResourceManager rm;
		Actor player;
		GameItem item;
		GameItem sword;

		void buildDefaultPossession(Possession& posn)
		{
			posn.quantity.setValues(0, 99, 0);
			posn.item = &item;
		}

		TEST_METHOD_INITIALIZE(start)
		{
			initDefaultActor(player);
		}
		
		TEST_METHOD(gainMoney)
		{
			player.money.setCurr(0); //should be 0 by default, but this is more explicit

			int moneyGain = 100;
			alterStatValue(player.money, moneyGain);
			
			Assert::AreEqual(moneyGain, player.money.getCurr());
		}

		TEST_METHOD(maxOutHP) //like if using a restorative item or sleeping at an inn
		{
			int maxHP = 50;

			BoundInt* hp = &(player.stats.hp);

			hp->setCurrMax(maxHP);
			hp->setCurr(10);

			hp->maxOut();

			Assert::AreEqual(maxHP, hp->getCurr());
		}

		TEST_METHOD(gainExperience) //no level up
		{
			player.stats.exp.setCurr(100);
			
			player.alterStat(StatType::EXP, 10);
			
			Assert::AreEqual(110, player.stats.exp.getCurr());
		}

		TEST_METHOD(alterStatSimpleTest)
		{
			BoundInt& hp = player.getStat(StatType::HP);
			hp.setCurrMax(50);
			hp.setCurr(10);
			Assert::IsTrue(player.alterStat(StatType::HP, 30));			
		}

		TEST_METHOD(alterStatTest)
		{
			BoundInt& hp = player.getStat(StatType::HP);
			hp.setCurrMax(50);
			hp.setCurr(49);
			
			Assert::IsTrue(player.alterStat(StatType::HP, 30));
		}

		TEST_METHOD(alterStatFailTest)
		{
			BoundInt& hp = player.getStat(StatType::HP);
			hp.setCurrMax(50);
			hp.setCurr(50);

			Assert::IsFalse(player.alterStat(StatType::HP, 30));
		}

		TEST_METHOD(alterStatNegativeFailTest)
		{
			BoundInt& hp = player.getStat(StatType::HP);
			hp.setCurr(0);

			Assert::IsFalse(player.alterStat(StatType::HP, -30));
		}

		TEST_METHOD(damageActor)
		{
			player.stats.hp.setCurr(25);

			player.alterStat(StatType::HP, -10);

			Assert::AreEqual(15, player.stats.hp.getCurr());
		}

		TEST_METHOD(healActor)
		{
			int maxHP = 25;
			player.stats.hp.setCurrMax(maxHP);
			player.stats.hp.setCurr(1);

			player.alterStat(StatType::HP, 30); //player is healed beyond max

			Assert::AreEqual(maxHP, player.stats.hp.getCurr()); 
		}

		TEST_METHOD(isActorAlive)
		{
			Assert::IsTrue(isAlive(player));
		}

		TEST_METHOD(isActorDead)
		{
			player.alterStat(StatType::HP, -10000);

			Assert::IsFalse(isAlive(player));
		}
		

		//actor gets status affliction
		//actor cures status affliction
		//gain experience with level gain
		
		//actor ingests consumable (could be meat, drink, or supplies)
		TEST_METHOD(ingestConsumableTest)
		{
			player.getStat(StatType::HP).setValues(0, 20, 9);
			
			Possession possession;
			buildDefaultPossession(possession);
			possession.quantity.setCurr(1);
			
			GameItem* item = possession.item;
			item->type = GameItemType::USABLE;
			
			item->effects.statValues.insert(std::make_pair(StatType::HP, 10));
			
			player.ingestConsumable(possession);

			Assert::AreEqual(19, player.getStat(StatType::HP).getCurr());
			Assert::AreEqual(0, possession.quantity.getCurr());
		}

		TEST_METHOD(ingestConsumableMaxOutFailTest) //consumable is not ingested because player has max hp for example
		{
			player.getStat(StatType::HP).setValues(0, 20, 20);

			Possession possession;
			buildDefaultPossession(possession);
			possession.quantity.setCurr(1);

			GameItem* item = possession.item;
			item->type = GameItemType::USABLE;

			item->effects.statValues.insert(std::make_pair(StatType::HP, 10));

			player.ingestConsumable(possession);

			Assert::AreEqual(1, possession.quantity.getCurr());
		}

		TEST_METHOD(ingestEquippableFailTest)
		{
			player.getStat(StatType::HP).setValues(0, 20, 9);

			Possession possession;
			buildDefaultPossession(possession);
			possession.item = &sword;
			possession.quantity.setCurr(1);

			GameItem* item = possession.item;
			item->type = GameItemType::EQUIPPABLE;

			item->effects.statValues.insert(std::make_pair(StatType::STRENGTH, 10));

			player.ingestConsumable(possession);
			Assert::AreEqual(1, possession.quantity.getCurr());
		}

		TEST_METHOD(equipWeaponTest)
		{
			player.getStat(StatType::STRENGTH).setValues(0, 255, 15);
			
			Possession possession;
			buildDefaultPossession(possession);
			possession.item = &sword;
			possession.quantity.setCurr(1);

			GameItem* item = possession.item;
			item->type = GameItemType::EQUIPPABLE;

			item->effects.statValues.insert(std::make_pair(StatType::STRENGTH, 30));
			item->part = EquipPart::WEAPON;
			player.equip(possession);
			
			Assert::AreEqual(45, player.getStat(StatType::STRENGTH).getCurr());
			Assert::AreEqual((int)item, (int)player.weapon);
		}

		TEST_METHOD(unEquipNothingTest)
		{
			int startStrength = 15;
			player.getStat(StatType::STRENGTH).setValues(0, 255, startStrength);

			GameItem* item = player.unEquip(EquipPart::WEAPON);

			Assert::AreEqual(startStrength, player.getStat(StatType::STRENGTH).getCurr());
			Assert::IsNull(item);
		}

		TEST_METHOD(unEquipWeaponTest)
		{
			int startStrength = 15;
			player.getStat(StatType::STRENGTH).setValues(0, 255, startStrength);

			Possession possession;
			buildDefaultPossession(possession);
			possession.item = &sword;
			possession.quantity.setCurr(1);

			GameItem* item = possession.item;
			item->type = GameItemType::EQUIPPABLE;

			item->effects.statValues.insert(std::make_pair(StatType::STRENGTH, 30));
			item->part = EquipPart::WEAPON;
			player.equip(possession);
			GameItem* removedItem = player.unEquip(EquipPart::WEAPON);

			Assert::AreEqual(startStrength, player.getStat(StatType::STRENGTH).getCurr());
			Assert::AreEqual((int)item, (int)removedItem);
			Assert::IsNull(player.weapon);
		}


	};
}