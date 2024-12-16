#include "mawJLaygoTitleTree.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/titleScene.h"
#include "manager/objectManager.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorNode/selectorNode .h"
#include "behaviorTaskNodes/roaringInfinityTask.h"

void MawJLaygoTitleTree::CreateTree(BossEnemy* boss)
{
	if (boss == nullptr) return;
	TitleScene* scene = SceneManager::GetScene<TitleScene>();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;

	// -------------------------- �r�w�C�r�A�c���[�̍쐬 --------------------------

	SelectorNode* rootNode = new SelectorNode;
	if (rootNode == nullptr) return;

	rootNode->AddTaskChild<RoaringInfinityTask>(boss, nullptr);

	// ��ԍŌ��
	CreateRoot(rootNode);
}
