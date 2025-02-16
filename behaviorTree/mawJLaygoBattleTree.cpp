#include "mawJLaygoBattleTree.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "manager/objectManager.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorNode/selectorNode.h"
#include "behaviorNode/sequenceNode.h"
#include "behaviortaskNode/dashAtThePlayerTask.h"
#include "behaviorTaskNode/leftSwipingTask.h"
#include "behaviorTaskNode/rightSwipingTask.h"
#include "behaviortaskNode/checkRangeTask.h"
#include "behaviortaskNode/roaringTask.h"
#include "behaviorTaskNode/checkHealthTask.h"
#include "behaviorTaskNode/deadTask.h"
#include "behaviorTaskNode/jumpAttackTask.h"
#include "behaviorTaskNode/waitTask.h"
#include "behaviorTaskNode/checkAttackParry.h"
#include "behaviorTaskNode/parryRecoilTask.h"
#include "behaviorTaskNode/airLightningBallTask.h"
#include "behaviorTaskNode/rightPunchTask.h"
#include "behaviorTaskNode/shortRangeAttackTask.h"
#include "behaviorTaskNode/backJumpTask.h"
#include "behaviorTaskNode/lightningBattlecryTask.h"
#include "behaviorTaskNode/middleRangeAttackTask.h"
#include "behaviorTaskNode/lightningFallFowardRainTask.h"

void MawJLaygoBattleTree::CreateTree(BossEnemy* boss)
{
	if (boss == nullptr) return;
	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;
	Player* player = objectManager->GetPlayer();
	if (player == nullptr) return;

	// -------------------------- ビヘイビアツリーの作成 --------------------------

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	BehaviorNode* healthSeqNode = rootNode->AddNodeChild<SequenceNode>("体力管理シーケンス");
	if (healthSeqNode == nullptr) return;

	// 咆哮
	healthSeqNode->AddTaskChild<RoaringTask>(boss, player);
	// 体力の確認
	healthSeqNode->AddTaskChild<CheckHealthTask>(boss, player);
	// 死亡
	healthSeqNode->AddTaskChild<DeadTask>(boss, player);

	BehaviorNode* parryRecoilNode = rootNode->AddNodeChild<SequenceNode>("パリィ管理シーケンス");
	if (parryRecoilNode == nullptr) return;

	// パリィの確認
	BehaviorNode* ceckParry = parryRecoilNode->AddTaskChild<CheckAttackParryTask>(boss, player);
	if (ceckParry == nullptr) return;
	// パリィされた
	ceckParry->AddTaskChild<ParryRecoilTask>(boss, player);


	// ▼デバッグ用▼
	
	// rootNode->AddTaskChild<LightningFallFowardRainTask>(boss, player);

	//DERIVATION_DATA deb = { 1.0f,100,1.0f };
	//BehaviorNode* debug1 = rootNode->AddTaskChild<AirLightningBallTask>(boss, player);
	//BehaviorNode* debug2 = debug1->AddTaskChild<AirLightningBallTask>(deb,boss, player);
	//BehaviorNode* debug3 = debug2->AddTaskChild<AirLightningBallTask>(deb,boss, player);
	//DERIVATION_DATA derivToBackJump = { 1.0f,100,0.2f };
	//debug3->AddTaskChild<JumpAttackTask>(derivToBackJump, boss, player);

	// ▲デバッグ用▲


	BehaviorNode* attackSelNode = rootNode->AddNodeChild<SelectorNode>("攻撃管理セレクター");
	if (attackSelNode == nullptr) return;

	// 待機
	attackSelNode->AddTaskChild<WaitTask>(boss, player);
	// 範囲の確認
	attackSelNode->AddTaskChild<CheckRangeTask>(boss, player);
	// 攻撃

	// 近距離
	if (BehaviorNode* shortAttackTask = attackSelNode->AddTaskChild<ShortRangeAttackTask>(boss, player))
	{
		shortAttackTask->AddTaskChild<RightPunchTask>(25, boss, player);
		shortAttackTask->AddTaskChild<BackJumpTask>(10, boss, player);
		DERIVATION_DATA lightningBarstDerivData = { 0.6f,100,0.0f };
		shortAttackTask->AddTaskChild<LightningBattlecryTask>(lightningBarstDerivData,5, boss, player);
		if (BehaviorNode* leftSwiping = shortAttackTask->AddTaskChild<LeftSwipingTask>(60, boss, player))
		{
			DERIVATION_DATA derivToRightSwiping = { 0.7f,100,0.7f };
			DERIVATION_DATA derivToBackJump = { 0.7f,100,0.4f };

			leftSwiping->AddTaskChild<RightSwipingTask>(derivToRightSwiping, 80, boss, player);
			if (BehaviorNode* backJump = leftSwiping->AddTaskChild<BackJumpTask>(derivToBackJump, 20, boss, player))
			{
				DERIVATION_DATA lightningBDerivData = { 0.5f, 50 ,0.85f };
				backJump->AddTaskChild<LightningFallFowardRainTask>(lightningBDerivData, boss, player);
			}
		}
	}


	// 中距離
	if (BehaviorNode* middleAttackTask = attackSelNode->AddTaskChild<MiddleRangeAttackTask>(boss, player))
	{
		// ジャンプ攻撃派生
		if (BehaviorNode* jumpAttack = middleAttackTask->AddTaskChild<JumpAttackTask>(75,boss, player))
		{
			DERIVATION_DATA lightningBallDerivData = { 0.75f,70 ,0.3f };
			jumpAttack->AddTaskChild<AirLightningBallTask>(lightningBallDerivData, 50, boss, player);
			DERIVATION_DATA lightningBarstDerivData = { 0.7f,70,0.6f };
			jumpAttack->AddTaskChild<LightningBattlecryTask>(lightningBarstDerivData, 40, boss, player);
			// 少し早い
			jumpAttack->AddTaskChild<LightningBattlecryTask>(lightningBarstDerivData, 10, boss, player);
		}

		// 連続雷弾 / 2段目,3段目キャンセル
		if (BehaviorNode* lb1 = middleAttackTask->AddTaskChild<AirLightningBallTask>(20,boss, player))
		{
			DERIVATION_DATA lightningBallDerivData = { 0.83f,100,1.0f };
			if (BehaviorNode* lb2 = lb1->AddTaskChild<AirLightningBallTask>(lightningBallDerivData, boss, player))
			{
				DERIVATION_DATA jumpAttackDerivData = { 0.68f,100,0.2f };
				lb2->AddTaskChild<JumpAttackTask>(jumpAttackDerivData, 25, boss, player);

				if (BehaviorNode* lb3 = lb2->AddTaskChild<AirLightningBallTask>(lightningBallDerivData, 75, boss, player))
				{
					lb3->AddTaskChild<JumpAttackTask>(jumpAttackDerivData, boss, player);
				}		
			}
		}
		// 雷前方攻撃
		middleAttackTask->AddTaskChild<LightningFallFowardRainTask>(5,boss, player);
	}

	// 攻撃不可
	attackSelNode->AddTaskChild<DashAtThePlayerTask>(boss, player);

	// 一番最後に
	CreateRoot(rootNode);
}
