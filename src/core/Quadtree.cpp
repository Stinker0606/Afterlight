//
// Created by $Will on 12.06.2025.
//

#include "Quadtree.h"
#include "../game/Collidable.h"

Quadtree::Quadtree(int pLevel, Rectangle pBounds)
    : level(pLevel), bounds(pBounds) {}

// Rekursive Methode zum Aufräumen des Baumes nach jedem Frame
void Quadtree::Clear()
{
    objects.clear();

    for (int i = 0; i < 4; i++)
    {
        if (nodes[i])
        {
            nodes[i] ->Clear();
            nodes[i].reset();
        }
    }
}


// Spaltet den aktuellen Knoten in 4 neue Knoten welche je 1/4 der Größe dess Knotens davor haben
void Quadtree::Split()
{
    float subWidth = bounds.width / 2;
    float subHeight = bounds.height / 2;
    float x = bounds.x;
    float y = bounds.y;

    nodes[0] = std::make_unique<Quadtree>(level + 1, Rectangle{ x, y, subWidth, subHeight });
    nodes[1] = std::make_unique<Quadtree>(level + 1, Rectangle{ x + subWidth, y, subWidth, subHeight });
    nodes[2] = std::make_unique<Quadtree>(level + 1, Rectangle{ x, y + subHeight, subWidth, subHeight });
    nodes[3] = std::make_unique<Quadtree>(level + 1, Rectangle{ x + subWidth, y + subHeight, subWidth, subHeight });
}

// Methode zur Bestimmung in welchem Knoten unser Objekt liegen sollte
int Quadtree::Get_Index(Collidable* object)
{
    // inxes = -1 bedeutet erstmal, dass er damit nicht in den aktuellen Knoten passt
    int index = -1;

    // wir nehmen die Grenzen des Objekts anhand der Hitbox
    Rectangle objectBounds = object->Get_Hitbox();

    // Wir stellen fest, wo der Mittelpunkt unseres aktuellen Knoten liegt
    float verticalMidpoint = bounds.x + (bounds.width / 2.0f);
    float horizontalMidpoint = bounds.y + (bounds.height / 2.0f);

    // Prüfung ob Objekt in eines der 4 Knoten passt, sonst bleibt es im alten Knoten wenn es auf einer Linie liegt
    bool fitsInTop = (objectBounds.y < horizontalMidpoint && objectBounds.y + objectBounds.height < horizontalMidpoint);
    bool fitsInBottom = (objectBounds.y > horizontalMidpoint);

    if (objectBounds.x < verticalMidpoint && objectBounds.x + objectBounds.width < verticalMidpoint)
    {
        if (fitsInTop)
        {
            index = 0;
        }
        else if (fitsInBottom)
        {
            index = 2;
        }
    }

    else if (objectBounds.x > verticalMidpoint)
    {
        if (fitsInTop)
        {
            index = 1;
        }
        else if (fitsInBottom)
        {
            index = 3;
        }
    }

    return index;
}

// Soll den Quadtree jeden Frame mit Objekten befüllen
void Quadtree::Insert(Collidable* object)
{
    if (nodes[0])
    {
        int index = Get_Index(object);
        if (index != -1)
        {
            nodes[index]->Insert(object);
            return;
        }
    }

    // Sobald das Objekt den tiefst möglichen Punkt erreicht hat, wir es über diese Methode in der Ebene gespeichert
    objects.push_back(object);


    if (objects.size() > max_Objects && level < max_Levels)
    {
        if (!nodes[0])  // wenn dieser Knoten noch keine Kind Knoten hat... nullptr gibt false zurück, hier soll
                        // er true zurück geben also "!"
        {
            Split();
        }

        // Setzt die neue Ebene auf 0 und beginnt Objekte aus einem höheren Knoten hier hinein zu drücken wenn es geht
        int i = 0;
        while (i <objects.size())
        {
            int index = Get_Index(objects[i]);

            if (index != -1) // wenn das Objekt in einen Kindknoten passt
            {
                // verschiebe es dort hinein
                nodes[index]->Insert(objects[i]);
                // lösche es aus dem aktuellen Knoten
                objects.erase(objects.begin() + i);
            }
            else
            {
                i++;
            }
        }
    }
}

// sucht für ein Objekt alle Objekte heraus welche mit diesem kollidieren könnten
std::vector<Collidable*> Quadtree::Retrieve(Collidable* object)
{
    std::vector<Collidable*> return_Objects;

    // Wo liegt unser Objekt
    int index = Get_Index(object);

    // wenn der Knoten geteilt wurde (kein nullptr) und das Objekt nicht auf einer Linie liegt, geht er eine Ebene
    // tiefer und prüft da genau das gleiche
    if (nodes[0] && index != -1)
    {
        return_Objects = nodes[index]->Retrieve(object);
    }

    // Speichert die Objekte auf dem Weg die nicht tiefer konnten zusätzlich zum Vektor
    return_Objects.insert(return_Objects.end(), objects.begin(), objects.end());
    return return_Objects;
}