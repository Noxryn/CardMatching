#include "CardView.h"
#include "configs/models/CardResConfig.h"
#include <iostream>
USING_NS_CC;

CardView* CardView::create(const CardModel* cardModel)
{
    CardView* ret = new (std::nothrow) CardView();
    if (ret && ret->init(cardModel))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool CardView::init(const CardModel* cardModel)
{
    if (!Node::init())
    {
        return false;
    }

    _cardModel = cardModel;

    std::string path = CardResConfig::getCardGeneralPath();
    _generalPng = Sprite::create(path);
    if (!_generalPng)
    {
        std::cout << "generalPng path" << std::endl;
        return false;
    }

    _generalPng->setScale(5.0f);

    this->addChild(_generalPng);

    auto originalSize = _generalPng->getContentSize();
    this->setContentSize(Size(originalSize.width * 5.0f, originalSize.height * 5.0f));

    _generalPng->setPosition(this->getContentSize().width / 2,
        this->getContentSize().height / 2);

    path = CardResConfig::getNumberPath(
        cardModel->getFace(),
        cardModel->getSuit(),
        false
    );
    _smallNumberPng = Sprite::create(path);
    if (!_smallNumberPng)
    {
        std::cout << "smallNumberPng path" << std::endl;
        return false;
    }

    _smallNumberPng->setScale(5.0f);

    this->addChild(_smallNumberPng);

    _smallNumberPng->setPosition(this->getContentSize().width * 0.2f,
        this->getContentSize().height * 0.8f);

    path = CardResConfig::getSuitPath(cardModel->getSuit());
    _suitPng = Sprite::create(path);
    if (!_suitPng)
    {
        std::cout << "suitPng path" << std::endl;
        return false;
    }
    _suitPng->setScale(5.0f);

    this->addChild(_suitPng);

    _suitPng->setPosition(this->getContentSize().width * 0.8f,
        this->getContentSize().height * 0.8f);

    path = CardResConfig::getNumberPath(
        cardModel->getFace(),
        cardModel->getSuit(),
        true
    );
    _bigNumberPng = Sprite::create(path);
    if (!_bigNumberPng)
    {
        std::cout << "bigNumberPng path" << std::endl;
        return false;
    }

    _bigNumberPng->setScale(5.0f);

    this->addChild(_bigNumberPng);

    _bigNumberPng->setPosition(this->getContentSize().width / 2,
        this->getContentSize().height / 2);

    this->setPosition(cardModel->getPosition());

    return true;
}

void CardView::updateCard(const CardModel* cardModel)
{
    _cardModel = cardModel;
    std::string path = CardResConfig::getNumberPath(
        cardModel->getFace(),
        cardModel->getSuit(),
        false
    );

    Texture2D* smallNumberTexture = Director::getInstance()->getTextureCache()->addImage(path);
    if (smallNumberTexture)
    {
        _smallNumberPng->setTexture(smallNumberTexture);
    }

    path = CardResConfig::getSuitPath(cardModel->getSuit());

    Texture2D* suitTexture = Director::getInstance()->getTextureCache()->addImage(path);
    if (suitTexture)
    {
        _suitPng->setTexture(suitTexture);
    }
    path = CardResConfig::getNumberPath(
        cardModel->getFace(),
        cardModel->getSuit(),
        true
    );

    Texture2D* numberTexture = Director::getInstance()->getTextureCache()->addImage(path);
    if (numberTexture)
    {
        _bigNumberPng->setTexture(numberTexture);
    }
    this->setPosition(cardModel->getPosition());
}

void CardView::moveToPosition(const cocos2d::Vec2& targetPos, float duration)
{
    auto moveAction = MoveTo::create(duration, targetPos);
    auto easeAction = EaseOut::create(moveAction, 2.0f);

    this->runAction(easeAction);
}
