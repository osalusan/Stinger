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
#include "behaviorTaskNode/lightningBarstTask.h"
#include "behaviorTaskNode/middleRangeAttackTask.h"
#include "behaviorTaskNode/lightningFallFowardRainTask.h"
#include "behaviorTaskNode/checkRenditionTask.h"

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

	BehaviorNode* renditionTask = rootNode->AddTaskChild<CheckRenditionTask>(boss, player);
	if (renditionTask == nullptr) return;

	// 演出用の咆哮
	//renditionTask->AddTaskChild<RoaringTask>(boss, player);

	BehaviorNode* healthSeqNode = rootNode->AddNodeChild<SequenceNode>("体力管理シーケンス");
	if (healthSeqNode == nullptr) return;

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
	
	// rootNode->AddTaskChild<LightningBarstTask>(boss, player);

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
		DERIVATION_DATA lightningBarstDerivData = { 0.6f,100,0.0f };
		shortAttackTask->AddTaskChild<LightningBarstTask>(lightningBarstDerivData, 5, boss, player);
		// バックジャンプ派生
		if (BehaviorNode* backJump = shortAttackTask->AddTaskChild<BackJumpTask>(25, boss, player))
		{
			DERIVATION_DATA backJumpTo{ 0.6f,100,0.9f };
			backJump->AddTaskChild<LightningFallFowardRainTask>(backJumpTo,60, boss, player);
			backJump->AddTaskChild<JumpAttackTask>(backJumpTo,40, boss, player);
		}

		// 左振りかざし派生
		if (BehaviorNode* leftSwiping = shortAttackTask->AddTaskChild<LeftSwipingTask>(45, boss, player))
		{
			DERIVATION_DATA derivToRightSwiping = { 0.7f,100,0.7f };
			DERIVATION_DATA derivToRightSwipingQuick = { 0.5f,100,0.35f };
			DERIVATION_DATA derivToBackJump = { 0.6f,100,0.4f };
			DERIVATION_DATA derivToLightningBarst = { 0.5f,80,0.4f };

			leftSwiping->AddTaskChild<RightSwipingTask>(derivToRightSwiping, 20, boss, player);
			leftSwiping->AddTaskChild<RightSwipingTask>(derivToRightSwipingQuick, 20, boss, player);
			leftSwiping->AddTaskChild<LightningBarstTask>(derivToLightningBarst, 15, boss, player);
			if (BehaviorNode* backJump = leftSwiping->AddTaskChild<BackJumpTask>(derivToBackJump, 45, boss, player))
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
			const int& jumpAChance = 70;
			DERIVATION_DATA lightningBallDerivData = { 0.75f,jumpAChance,0.3f };
			jumpAttack->AddTaskChild<AirLightningBallTask>(lightningBallDerivData, 50, boss, player);
			DERIVATION_DATA lightningBarstDerivData1 = { 0.7f,jumpAChance,0.6f };
			jumpAttack->AddTaskChild<LightningBarstTask>(lightningBarstDerivData1, 40, boss, player);
			// 上のライトニングバーストより発生が少し早い
			DERIVATION_DATA lightningBarstDerivData2 = { 0.5f,jumpAChance,0.45f };
			jumpAttack->AddTaskChild<LightningBarstTask>(lightningBarstDerivData2, 10, boss, player);
		}

		// 連続雷球 / 2段目,3段目キャンセル
		if (BehaviorNode* lb1 = middleAttackTask->AddTaskChild<AirLightningBallTask>(20,boss, player))
		{
			DERIVATION_DATA lightningBallDerivData = { 0.83f,100,1.0f };
			// ２発目
			if (BehaviorNode* lb2 = lb1->AddTaskChild<AirLightningBallTask>(lightningBallDerivData, boss, player))
			{
				DERIVATION_DATA jumpAttackDerivData = { 0.68f,100,0.2f };
				lb2->AddTaskChild<JumpAttackTask>(jumpAttackDerivData, 25, boss, player);

				// ３発目
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
