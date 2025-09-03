#include "Items/Inv_ItemTags.h"

namespace GameItems
{
	namespace Equipment
	{
		namespace Weapons
		{
			UE_DEFINE_GAMEPLAY_TAG(Axe, "GameItems.Equipment.Weapons.Axe");
			UE_DEFINE_GAMEPLAY_TAG(Sword, "GameItems.Equipment.Weapons.Sword");
		}

		namespace Cloaks
		{
			UE_DEFINE_GAMEPLAY_TAG(RedCloak, "GameItems.Equipment.Cloaks.RedCloak");
		}

		namespace Masks
		{
			UE_DEFINE_GAMEPLAY_TAG(SteelMask, "GameItems.Equipment.Masks.SteelMask");
		}
	}

	namespace Consumables
	{
		namespace Potions
		{
			namespace Red
			{
				UE_DEFINE_GAMEPLAY_TAG(Small, "GameItems.Consumable.Potions.Red.Small");
				UE_DEFINE_GAMEPLAY_TAG(Large, "GameItems.Consumable.Potions.Red.Large");
			}

			namespace Blue
			{
				UE_DEFINE_GAMEPLAY_TAG(Small, "GameItems.Consumable.Potions.Blue.Small");
				UE_DEFINE_GAMEPLAY_TAG(Large, "GameItems.Consumable.Potions.Blue.Large");
			}
		}
	}

	namespace Craftable
	{
		UE_DEFINE_GAMEPLAY_TAG(FireFernFruit, "GameItems.Craftable.FireFernFruit");
		UE_DEFINE_GAMEPLAY_TAG(LuminDaisy, "GameItems.Craftable.LuminDaisy");
		UE_DEFINE_GAMEPLAY_TAG(ScorchPetalBlossom, "GameItems.Craftable.ScorchPetalBlossom");
	}
}