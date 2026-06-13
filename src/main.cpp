#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(MyStatHUD, PlayLayer) {
    // Declaração das variáveis que vão armazenar os dados e os textos
    int m_clicks = 0;
    float m_fpsTimer = 0.0f;
    int m_fpsFrames = 0;
    
    CCLabelBMFont* m_clickLabel = nullptr;
    CCLabelBMFont* m_fpsLabel = nullptr;
    CCLabelBMFont* m_bestAttemptLabel = nullptr;

    bool init(GJGameLevel* level, bool useReplay, bool dontRun) {
        if (!PlayLayer::init(level, useReplay, dontRun)) return false;

        // Resetar o contador de cliques ao iniciar o level
        m_clicks = 0;

        // Pegar o tamanho da tela para posicionar os elementos corretamente
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // 1. Criar e posicionar o texto de Cliques (Canto superior esquerdo)
        m_clickLabel = CCLabelBMFont::create("Clicks: 0", "bigFont.fnt");
        m_clickLabel->setScale(0.4f);
        m_clickLabel->setAnchorPoint({0, 1}); // Alinha pelo topo-esquerda
        m_clickLabel->setPosition({10, winSize.height - 10});
        this->addChild(m_clickLabel, 999);

        // 2. Criar e posicionar o texto de FPS (Logo abaixo dos cliques)
        m_fpsLabel = CCLabelBMFont::create("FPS: --", "bigFont.fnt");
        m_fpsLabel->setScale(0.4f);
        m_fpsLabel->setAnchorPoint({0, 1});
        m_fpsLabel->setPosition({10, winSize.height - 25});
        this->addChild(m_fpsLabel, 999);

        // 3. Pegar a melhor tentativa do nível atual e mostrar (Abaixo do FPS)
        // O RobTop salva o progresso normal em 'm_normalPercent' dentro do objeto do level
        int bestPercent = level->m_normalPercent;
        std::string bestText = "Melhor: " + std::to_string(bestPercent) + "%";
        
        m_bestAttemptLabel = CCLabelBMFont::create(bestText.c_str(), "bigFont.fnt");
        m_bestAttemptLabel->setScale(0.4f);
        m_bestAttemptLabel->setAnchorPoint({0, 1});
        m_bestAttemptLabel->setPosition({10, winSize.height - 40});
        this->addChild(m_bestAttemptLabel, 999);

        // Ativar a função 'update' para rodar a cada frame (necessário para o FPS)
        this->scheduleUpdate();

        return true;
    }

    // Função que intercepta o clique do jogador
    void pushButton(PlayerButton btn, bool isPlayer2) {
        PlayLayer::pushButton(btn, isPlayer2);
        
        // Só conta se for o Player 1 pulando (evita bugar em duas pessoas ou com comandos de veículo)
        if (btn == PlayerButton::Jump && !isPlayer2) {
            m_clicks++;
            if (m_clickLabel) {
                m_clickLabel->setString(fmt::format("Clicks: {}", m_clicks).c_str());
            }
        }
    }

    // Função que roda a cada frame do jogo
    void update(float dt) {
        PlayLayer::update(dt);

        // Lógica para calcular o FPS de forma estável (atualiza a cada 0.5 segundos)
        m_fpsFrames++;
        m_fpsTimer += dt;

        if (m_fpsTimer >= 0.5f) {
            int currentFps = std::round(m_fpsFrames / m_fpsTimer);
            
            if (m_fpsLabel) {
                m_fpsLabel->setString(fmt::format("FPS: {}", currentFps).c_str());
            }

            // Resetar o timer do FPS
            m_fpsFrames = 0;
            m_fpsTimer = 0.0f;
        }
    }
};

