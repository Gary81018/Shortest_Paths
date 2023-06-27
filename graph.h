#ifndef GRAPH_H
#define GRAPH_H

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QTimer>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <cmath>
#include <random>
#include <iostream>
#include <cstring>

class graph : public QObject
{
    Q_OBJECT
private:


    QVector<QPointF>vertexs;

    QVector<QPair<int, int>>edges;
    QVector<int>weights;

    QVector<QGraphicsLineItem *> edgeItems;
    QVector<QGraphicsLineItem *> edgeItems_b;
    QVector<QGraphicsTextItem *> textItems;

    double r;

    void paint_vertex(int i,QGraphicsScene *scene,Qt::GlobalColor color)
    {
        QGraphicsEllipseItem *item = new QGraphicsEllipseItem(-r, -r, 2*r, 2*r);
        item->setPos(vertexs[i]);
        item->setBrush(color);
        scene->addItem(item);

        QGraphicsTextItem *text = new QGraphicsTextItem(QString::number(i + 1));
        text->setPos(vertexs[i]+QPointF(-r*0.4,-r*0.8));
        QFont font;
        font.setBold(true);
        font.setPointSize(24);
        text->setFont(font);
        scene->addItem(text);
    }

    void paint_edge(QGraphicsLineItem *item,QGraphicsScene *scene,Qt::GlobalColor color,double ra)
    {
        QPen pen;
        pen.setWidth(r*ra);
        pen.setBrush(color);
        item->setPen(pen);
        scene->addItem(item);
    }

    void blink_edge(QGraphicsLineItem *item,Qt::GlobalColor color,double ra,int zvalue)
    {
        QPen pen;
        pen.setWidth(r*ra);
        pen.setBrush(color);
        item->setPen(pen);
        item->setZValue(zvalue);
    }

public:

    void build_graph(){
        r=30;
        vertexs.clear();edges.clear();weights.clear();
        edgeItems.clear();edgeItems_b.clear();textItems.clear();
        freopen("data.in","r",stdin);
        int n,m;
        srand(time(0));
        scanf("%d%d",&n,&m);
        for (int i=0;i<m;i++)
        {
            int x,y,z;
            scanf("%d%d%d",&x,&y,&z);
            x--;y--;
            edges.append(std::make_pair(x,y));
            weights.append(z);
        }
        double x0=400.0,y0=300.0,angle=360.0/n;
        if (n%2==0) angle=360.0/(n-1);
        for (int i=0;i<n;i++)
        {
            if (n%2==0&&i==n-1) vertexs.append(QPointF(x0,y0));
            else vertexs.append(QPointF(x0+350*std::cos(i*angle*M_PI/180),y0+250*std::sin(i*angle*M_PI/180)));
        }
        for (int i=0;i<m;i++)
        {
            QLineF line(vertexs[edges[i].first],vertexs[edges[i].second]);
        }
        fclose(stdin);
    };

    int getN()
    {
        return vertexs.size();
    }

    void refresh()
    {
        for (int i=0;i<edges.size();i++)
        {
            QGraphicsLineItem *e_item=edgeItems[i];
            QGraphicsTextItem *t_item=textItems[i];
            QGraphicsLineItem *eb_item=edgeItems_b[i];
            t_item->setZValue(100);
            blink_edge(eb_item,Qt::black,0.8,100);
            blink_edge(e_item,Qt::gray,0.74,100);
        }
    }

    QGraphicsScene *print_graph()
    {
        QGraphicsScene *scene = new QGraphicsScene;
        for (int i=0;i<edges.size();i++) {
            QLineF line(vertexs[edges[i].first], vertexs[edges[i].second]);
            qreal angle = line.angle() * M_PI / 180;
            qreal dx = r*1.35 * sin(angle);
            qreal dy = r*1.35 * cos(angle);
            line.setP1(line.p1() + QPointF(dy, -dx));
            line.setP2(line.p2() - QPointF(dy, -dx));

            QGraphicsLineItem *item = new QGraphicsLineItem(line);
            QGraphicsLineItem *item_b = new QGraphicsLineItem(line);
            edgeItems.append(item);
            edgeItems_b.append(item_b);
            paint_edge(item_b,scene,Qt::black,0.8);
            paint_edge(item,scene,Qt::gray,0.74);
        }

        for (int i=0;i<edges.size();i++)
        {
            QGraphicsTextItem *text = new QGraphicsTextItem(QString::number(weights[i]));
            text->setPos((vertexs[edges[i].first] + vertexs[edges[i].second]) / 2-QPointF(r*0.6,r*0.6));
            QFont font;
            font.setPointSize(15);
            text->setFont(font);
            scene->addItem(text);
            textItems.append(text);
        }

        for (int i = 0; i < vertexs.size(); ++i) {
            paint_vertex(i,scene,Qt::yellow);
        }
        return scene;
    }

    QVector<int> shortest_path(int st,int en)
    {
        int n=vertexs.size();
        int distance[n],fr[n];
        bool vis[n];
        QVector<int>shortest_paths;
        memset(distance,0x3f,sizeof(distance));
        memset(fr,-1,sizeof(fr));
        memset(vis,0,sizeof(vis));
        distance[st-1]=0;
        int inf=0x3f3f3f3f;
        for (int i=0;i<n;i++)
        {
            int p=-1;
            for (int j=0;j<n;j++) if (!vis[j])
            {
                if (p==-1||distance[j]<distance[p]) p=j;
            }
            if (p==-1||distance[p]==inf) break;
            if (fr[p]!=-1) shortest_paths.append(fr[p]);
            vis[p]=true;
            for (int j=0;j<edges.size();j++)
            {
                if (edges[j].first==p||edges[j].second==p)
                {
                    int v=edges[j].first^edges[j].second^p;
                    distance[v]=fmin(distance[v],distance[p]+weights[j]);
                    if (distance[v]==distance[p]+weights[j]) fr[v]=j;
                }
            }
        }

        shortest_paths.append(-1);
        QVector<int>stk;
        int cur=en-1;
        while (cur!=st-1)
        {

            for (int j=0;j<edges.size();j++)
            {
                if (edges[j].first==cur||edges[j].second==cur)
                {
                    int v=edges[j].first^edges[j].second^cur;
                    if (distance[v]+weights[j]==distance[cur])
                    {
                        stk.append(j);
                        cur=v;
                        break;
                    }
                }
            }
        }
        for (int i=stk.size()-1;i>=0;i--) shortest_paths.append(stk[i]);
        std::reverse(shortest_paths.begin(),shortest_paths.end());
        return shortest_paths;
    }

    void show_paths(QVector<int>paths)
    {
        int fl=0;
        for (int i=paths.size()-1;i>=0;i--) if (paths[i]==-1) fl=i;
        for (int i=paths.size()-1;i>=0;i--)
        {
            QTimer::singleShot((paths.size() - i - 1) * 1200, [=](){
                if (paths[i]!=-1)
                {
                    QGraphicsLineItem *e_item=edgeItems[paths[i]];
                    QGraphicsTextItem *t_item=textItems[paths[i]];
                    QGraphicsLineItem *eb_item=edgeItems_b[paths[i]];
                    t_item->setZValue(100);
                    blink_edge(eb_item,Qt::black,0.8,100);
                    blink_edge(e_item,Qt::red,0.74,100);
                    QTimer::singleShot(600, [=](){
                        if (i>fl) blink_edge(e_item,Qt::magenta,0.74,100);
                        else blink_edge(e_item,Qt::green,0.74,100);

                    });

                    if (i==0)
                    {
                        QTimer::singleShot(1200, [=]() {
                            emit finished();
                        });
                    }
                }
            });
        }
        if (paths.size()==0)
        {
            QTimer::singleShot(1200, [=]() {
                emit finished();
            });
        }
    }
signals:
    void finished();
};

#endif // GRAPH_H
