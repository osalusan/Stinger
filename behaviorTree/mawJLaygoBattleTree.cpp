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

	// -------------------------- �r�w�C�r�A�c���[�̍쐬 --------------------------

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	BehaviorNode* healthSeqNode = rootNode->AddNodeChild<SequenceNode>("�̗͊Ǘ��V�[�P���X");
	if (healthSeqNode == nullptr) return;

	// ���K
	healthSeqNode->AddTaskChild<RoaringTask>(boss, player);
	// �̗͂̊m�F
	healthSeqNode->AddTaskChild<CheckHealthTask>(boss, player);
	// ���S
	healthSeqNode->AddTaskChild<DeadTask>(boss, player);

	BehaviorNode* parryRecoilNode = rootNode->AddNodeChild<SequenceNode>("�p���B�Ǘ��V�[�P���X");
	if (parryRecoilNode == nullptr) return;

	// �p���B�̊m�F
	BehaviorNode* ceckParry = parryRecoilNode->AddTaskChild<CheckAttackParryTask>(boss, player);
	if (ceckParry == nullptr) return;
	// �p���B���ꂽ
	ceckParry->AddTaskChild<ParryRecoilTask>(boss, player);


	// ���f�o�b�O�p��
	
	// rootNode->AddTaskChild<LightningFallFowardRainTask>(boss, player);

	//DERIVATION_DATA deb = { 1.0f,100,1.0f };
	//BehaviorNode* debug1 = rootNode->AddTaskChild<AirLightningBallTask>(boss, player);
	//BehaviorNode* debug2 = debug1->AddTaskChild<AirLightningBallTask>(deb,boss, player);
	//BehaviorNode* debug3 = debug2->AddTaskChild<AirLightningBallTask>(deb,boss, player);
	//DERIVATION_DATA derivToBackJump = { 1.0f,100,0.2f };
	//debug3->AddTaskChild<JumpAttackTask>(derivToBackJump, boss, player);

	// ���f�o�b�O�p��


	BehaviorNode* attackSelNode = rootNode->AddNodeChild<SelectorNode>("�U���Ǘ��Z���N�^�[");
	if (attackSelNode == nullptr) return;

	// �ҋ@
	attackSelNode->AddTaskChild<WaitTask>(boss, player);
	// �͈͂̊m�F
	attackSelNode->AddTaskChild<CheckRangeTask>(boss, player);
	// �U��

	// �ߋ���
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


	// ������
	if (BehaviorNode* middleAttackTask = attackSelNode->AddTaskChild<MiddleRangeAttackTask>(boss, player))
	{
		// �W�����v�U���h��
		if (BehaviorNode* jumpAttack = middleAttackTask->AddTaskChild<JumpAttackTask>(75,boss, player))
		{
			DERIVATION_DATA lightningBallDerivData = { 0.75f,70 ,0.3f };
			jumpAttack->AddTaskChild<AirLightningBallTask>(lightningBallDerivData, 50, boss, player);
			DERIVATION_DATA lightningBarstDerivData = { 0.7f,70,0.6f };
			jumpAttack->AddTaskChild<LightningBattlecryTask>(lightningBarstDerivData, 40, boss, player);
			// ��������
			jumpAttack->AddTaskChild<LightningBattlecryTask>(lightningBarstDerivData, 10, boss, player);
		}

		// �A�����e / 2�i��,3�i�ڃL�����Z��
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
		// ���O���U��
		middleAttackTask->AddTaskChild<LightningFallFowardRainTask>(5,boss, player);
	}

	// �U���s��
	attackSelNode->AddTaskChild<DashAtThePlayerTask>(boss, player);

	// ��ԍŌ��
	CreateRoot(rootNode);
}
