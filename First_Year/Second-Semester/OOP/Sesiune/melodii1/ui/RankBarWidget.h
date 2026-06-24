#pragma once

#include <QWidget>
#include <QPainter>
#include "../service/MelodieService.h"

class RankBarWidget : public QWidget
{
private:
    /*
     * serv: service reference
     */
    MelodieService& serv;

public:
    /*
     * Creates bar widget
     * :param serv: service reference
     */
    RankBarWidget(MelodieService& serv)
        : serv{serv}
    {
        setMinimumHeight(220);
    }

    /*
     * Draws bars for ranks 0..10
     */
    void paintEvent(QPaintEvent* event) override
    {
        QPainter p{this};

        auto freq = serv.rankFrequency();

        int leftMargin = 20;
        int bottomMargin = 30;
        int topMargin = 20;
        int spacing = 10;

        int n = 11; // rank 0..10
        int availableWidth = width() - 2 * leftMargin;
        int barWidth = (availableWidth - (n - 1) * spacing) / n;

        int maxFreq = 0;
        for (int x : freq)
        {
            if (x > maxFreq)
                maxFreq = x;
        }

        int usableHeight = height() - topMargin - bottomMargin;

        for (int i = 0; i < n; i++)
        {
            int barHeight = 0;
            if (maxFreq != 0)
            {
                barHeight = freq[i] * usableHeight / maxFreq;
            }

            int x = leftMargin + i * (barWidth + spacing);
            int y = height() - bottomMargin - barHeight;

            p.drawRect(x, y, barWidth, barHeight);

            // scriem rank-ul sub bară
            p.drawText(x, height() - 8, QString::number(i));

            // scriem frecvența deasupra barei
            p.drawText(x, y - 5, QString::number(freq[i]));
        }
    }
};