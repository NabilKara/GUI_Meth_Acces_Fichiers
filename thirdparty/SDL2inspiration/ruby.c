#include "EmeraldMine.h"
#include "explosion.h"
#include "magicwall.h"
#include "man.h"
#include "ruby.h"
#include "sound.h"

extern PLAYFIELD Playfield;

/*----------------------------------------------------------------------------
Name:           ControlRuby
------------------------------------------------------------------------------
Beschreibung: Steuert einen Rubin
Parameter
      Eingang: I, uint32_t, Index im Level
      Ausgang: -
R�ckgabewert:  -
Seiteneffekte: Playfield.x
------------------------------------------------------------------------------*/
void ControlRuby(uint32_t I) {
    uint8_t uFree;  // Richtung, in die Rubin rollen k�nnte: 0 = kann nicht rollen, 1 = kann links rollen, 2 = kann rechts rollen, 3 = kann links und rechts rollen
    uint16_t uHitElement;       // Element, welches getroffen wird
    uint32_t uHitCoordinate;    // Lineare Koordinate des getroffenen Elements

    // Doppelte Steuerung vermeiden
    if ((Playfield.pStatusAnimation[I] & 0x00FF0000) == EMERALD_ANIM_AVOID_DOUBLE_CONTROL) {
        Playfield.pStatusAnimation[I] = Playfield.pStatusAnimation[I] & 0xFF00FFFF;
        SDL_Log("%s: ack double control",__FUNCTION__);
        return;
    }
    if ((Playfield.pStatusAnimation[I] & 0xFF000000) == EMERALD_ANIM_SINK_IN_MAGIC_WALL) {
        Playfield.pLevel[I] = EMERALD_SPACE;    // Rubin ist in Magic Wall eingetaucht und wird zu Space
        Playfield.pStatusAnimation[I] = EMERALD_ANIM_STAND;
        return;
    }
    if ((Playfield.pStatusAnimation[I] & 0xFF000000) == EMERALD_ANIM_BLITZ) {
        Playfield.pStatusAnimation[I] = EMERALD_ANIM_STAND;
    }
    if (    ((Playfield.pStatusAnimation[I] & 0xFF000000) == EMERALD_ANIM_BORN1) ||
            ((Playfield.pStatusAnimation[I] & 0xFF000000) == EMERALD_ANIM_BORN2) ||
            ((Playfield.pStatusAnimation[I] & 0xFF000000) == EMERALD_ANIM_RUBY_SHRINK) ) {
        // Rubin kann vom Replikator geboren werden, dann hier nichts machen
        // Rubin kann durch Man "geshrinkt" werden, dann hier auch nichts machen
        return;
    } else if (Playfield.pLevel[I + Playfield.uLevel_X_Dimension] == EMERALD_SPACE) {   // Ist nach unten frei?
        // neuen Platz mit ung�ltigem Element besetzen
        Playfield.pLevel[I + Playfield.uLevel_X_Dimension] = EMERALD_INVALID;
        // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
        Playfield.pInvalidElement[I + Playfield.uLevel_X_Dimension] = EMERALD_RUBY;
        Playfield.pLastStatusAnimation[I + Playfield.uLevel_X_Dimension] = EMERALD_ANIM_DOWN;
        Playfield.pStatusAnimation[I + Playfield.uLevel_X_Dimension] = EMERALD_ANIM_DOWN_SELF | EMERALD_ANIM_CLEAN_UP;
        // Aktuelles Element auf Animation "unten"
        Playfield.pStatusAnimation[I] = EMERALD_ANIM_DOWN;
    } else if (Playfield.pLevel[I + Playfield.uLevel_X_Dimension] == EMERALD_ACIDPOOL) {   // F�llt Rubin ins S�urebecken?
        SDL_Log("Ruby falls in pool");
        Playfield.pLevel[I] = EMERALD_ACIDPOOL_DESTROY;
        Playfield.pInvalidElement[I] = EMERALD_RUBY;
        PreparePlaySound(SOUND_POOL_BLUB,I);
        return;
    } else {                            // Unten ist nicht frei
        // Rubin bleibt zun�chst auf Platz liegen
        uHitCoordinate = I + Playfield.uLevel_X_Dimension;
        uHitElement = Playfield.pLevel[uHitCoordinate];
        if ( (Playfield.pStatusAnimation[I] & 0xFF000000) == EMERALD_ANIM_DOWN_SELF) {
            Playfield.pStatusAnimation[I] &= 0x00FFFFFF;
            if ((uHitElement == EMERALD_MAGIC_WALL) || (uHitElement == EMERALD_MAGIC_WALL_STEEL)) { // Rubin trifft auf Magic wall
                if (Playfield.bMagicWallRunning) {
                    SDL_Log("Rubin hit running magic wall");
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_SINK_IN_MAGIC_WALL;
                    ElementGoesMagicWall(I,EMERALD_MEGABOMB);
                    PreparePlaySound(SOUND_SQUEAK,I);
                } else if ((!Playfield.bMagicWallWasOn) && (Playfield.uTimeMagicWall > 0)) {
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_SINK_IN_MAGIC_WALL;
                    SDL_Log("Rubin start magic wall");
                    Playfield.bMagicWallWasOn = true;
                    Playfield.uTimeMagicWallLeft = Playfield.uTimeMagicWall;
                    Playfield.bMagicWallRunning = true;
                    ElementGoesMagicWall(I,EMERALD_MEGABOMB);
                    PreparePlaySound(SOUND_SQUEAK,I);
                } else {
                    SDL_Log("Rubin hit used magic wall");
                    PreparePlaySound(SOUND_PING,I);
                }
            } else if (uHitElement == EMERALD_MAN) {
                SDL_Log("Rubin kills man");
                Playfield.pLevel[uHitCoordinate] = EMERALD_MAN_DIES;
                Playfield.pStatusAnimation[uHitCoordinate] = EMERALD_ANIM_AVOID_DOUBLE_CONTROL | EMERALD_ANIM_MAN_DIES_P1;
                PreparePlaySound(SOUND_MAN_CRIES,I);
                Playfield.bManDead = true;
            } else if (uHitElement == EMERALD_STANDMINE) {
                SDL_Log("Rubin hit stand mine");
                ControlCentralExplosion(uHitCoordinate);
                PreparePlaySound(SOUND_EXPLOSION,I);
            } else {
                PreparePlaySound(SOUND_PING,I);
            }
        }
        if ((Playfield.uRollUnderground[uHitElement] & EMERALD_CHECKROLL_RUBY) != 0) {
            uFree = GetFreeRollDirections(I);
            if (uFree == 1) {   // Rubin kann links rollen
                // neuen Platz mit ung�ltigem Element besetzen
                Playfield.pLevel[I - 1] = EMERALD_INVALID;
                // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                Playfield.pInvalidElement[I - 1] = EMERALD_RUBY;
                Playfield.pStatusAnimation[I - 1] = EMERALD_ANIM_CLEAN_RIGHT;
                // Aktuelles Element auf Animation "links"
                Playfield.pStatusAnimation[I] = EMERALD_ANIM_LEFT;

            } else if (uFree == 2) {    // Rubin kann rechts rollen
                // neuen Platz mit ung�ltigem Element besetzen
                Playfield.pLevel[I + 1] = EMERALD_INVALID;
                // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                Playfield.pInvalidElement[I + 1] = EMERALD_RUBY;
                Playfield.pLastStatusAnimation[I + 1] = EMERALD_ANIM_RIGHT;
                Playfield.pStatusAnimation[I + 1] = EMERALD_ANIM_CLEAN_LEFT;
                // Aktuelles Element auf Animation "rechts"
                Playfield.pStatusAnimation[I] = EMERALD_ANIM_RIGHT;
            } else if (uFree == 3) {    // Rubin kann in beide Richtungen rollen
                // Hier entscheiden, ob links oder rechts gerollt wird
                if ((rand() & 0x01) == 0) {   // links
                    // neuen Platz mit ung�ltigem Element besetzen
                    Playfield.pLevel[I - 1] = EMERALD_INVALID;
                    // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                    Playfield.pInvalidElement[I - 1] = EMERALD_RUBY;
                    Playfield.pStatusAnimation[I - 1] = EMERALD_ANIM_CLEAN_RIGHT;
                    // Aktuelles Element auf Animation "links"
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_LEFT;
                } else {                    // rechts
                    // neuen Platz mit ung�ltigem Element besetzen
                    Playfield.pLevel[I + 1] = EMERALD_INVALID;
                    // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                    Playfield.pInvalidElement[I + 1] = EMERALD_RUBY;
                    Playfield.pLastStatusAnimation[I + 1] = EMERALD_ANIM_RIGHT;
                    Playfield.pStatusAnimation[I + 1] = EMERALD_ANIM_CLEAN_LEFT;
                    // Aktuelles Element auf Animation "rechts"
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_RIGHT;
                }
            }
        } else {    // Ab hier pr�fen, ob Rubin durch Laufband bewegt werden kann
            if (uHitElement == EMERALD_CONVEYORBELT_RED) {
                if  ((Playfield.uConveybeltRedState == EMERALD_CONVEYBELT_LEFT) && (Playfield.pLevel[I - 1] == EMERALD_SPACE)) {
                    // neuen Platz mit ung�ltigem Element besetzen
                    Playfield.pLevel[I - 1] = EMERALD_INVALID;
                    // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                    Playfield.pInvalidElement[I - 1] = EMERALD_RUBY;
                    Playfield.pStatusAnimation[I - 1] = EMERALD_ANIM_CLEAN_RIGHT;
                    // Aktuelles Element auf Animation "links"
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_LEFT;
                } else if ((Playfield.uConveybeltRedState == EMERALD_CONVEYBELT_RIGHT) && (Playfield.pLevel[I + 1] == EMERALD_SPACE)) {
                    // neuen Platz mit ung�ltigem Element besetzen
                    Playfield.pLevel[I + 1] = EMERALD_INVALID;
                    // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                    Playfield.pInvalidElement[I + 1] = EMERALD_RUBY;
                    Playfield.pLastStatusAnimation[I + 1] = EMERALD_ANIM_RIGHT;
                    Playfield.pStatusAnimation[I + 1] = EMERALD_ANIM_CLEAN_LEFT;
                    // Aktuelles Element auf Animation "rechts"
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_RIGHT;
                }
            } else if (uHitElement == EMERALD_CONVEYORBELT_GREEN) {
                if  ((Playfield.uConveybeltGreenState == EMERALD_CONVEYBELT_LEFT) && (Playfield.pLevel[I - 1] == EMERALD_SPACE)) {
                    // neuen Platz mit ung�ltigem Element besetzen
                    Playfield.pLevel[I - 1] = EMERALD_INVALID;
                    // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                    Playfield.pInvalidElement[I - 1] = EMERALD_RUBY;
                    Playfield.pStatusAnimation[I - 1] = EMERALD_ANIM_CLEAN_RIGHT;
                    // Aktuelles Element auf Animation "links"
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_LEFT;
                } else if ((Playfield.uConveybeltGreenState == EMERALD_CONVEYBELT_RIGHT) && (Playfield.pLevel[I + 1] == EMERALD_SPACE)) {
                    // neuen Platz mit ung�ltigem Element besetzen
                    Playfield.pLevel[I + 1] = EMERALD_INVALID;
                    // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                    Playfield.pInvalidElement[I + 1] = EMERALD_RUBY;
                    Playfield.pLastStatusAnimation[I + 1] = EMERALD_ANIM_RIGHT;
                    Playfield.pStatusAnimation[I + 1] = EMERALD_ANIM_CLEAN_LEFT;
                    // Aktuelles Element auf Animation "rechts"
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_RIGHT;
                }
            } else if (uHitElement == EMERALD_CONVEYORBELT_BLUE) {
                if  ((Playfield.uConveybeltBlueState == EMERALD_CONVEYBELT_LEFT) && (Playfield.pLevel[I - 1] == EMERALD_SPACE)) {
                    // neuen Platz mit ung�ltigem Element besetzen
                    Playfield.pLevel[I - 1] = EMERALD_INVALID;
                    // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                    Playfield.pInvalidElement[I - 1] = EMERALD_RUBY;
                    Playfield.pStatusAnimation[I - 1] = EMERALD_ANIM_CLEAN_RIGHT;
                    // Aktuelles Element auf Animation "links"
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_LEFT;
                } else if ((Playfield.uConveybeltBlueState == EMERALD_CONVEYBELT_RIGHT) && (Playfield.pLevel[I + 1] == EMERALD_SPACE)) {
                    // neuen Platz mit ung�ltigem Element besetzen
                    Playfield.pLevel[I + 1] = EMERALD_INVALID;
                    // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                    Playfield.pInvalidElement[I + 1] = EMERALD_RUBY;
                    Playfield.pLastStatusAnimation[I + 1] = EMERALD_ANIM_RIGHT;
                    Playfield.pStatusAnimation[I + 1] = EMERALD_ANIM_CLEAN_LEFT;
                    // Aktuelles Element auf Animation "rechts"
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_RIGHT;
                }
            } else if (uHitElement == EMERALD_CONVEYORBELT_YELLOW) {
                if  ((Playfield.uConveybeltYellowState == EMERALD_CONVEYBELT_LEFT) && (Playfield.pLevel[I - 1] == EMERALD_SPACE)) {
                    // neuen Platz mit ung�ltigem Element besetzen
                    Playfield.pLevel[I - 1] = EMERALD_INVALID;
                    // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                    Playfield.pInvalidElement[I - 1] = EMERALD_RUBY;
                    Playfield.pStatusAnimation[I - 1] = EMERALD_ANIM_CLEAN_RIGHT;
                    // Aktuelles Element auf Animation "links"
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_LEFT;
                } else if ((Playfield.uConveybeltYellowState == EMERALD_CONVEYBELT_RIGHT) && (Playfield.pLevel[I + 1] == EMERALD_SPACE)) {
                    // neuen Platz mit ung�ltigem Element besetzen
                    Playfield.pLevel[I + 1] = EMERALD_INVALID;
                    // Damit ung�ltiges Feld sp�ter auf richtiges Element gesetzt werden kann
                    Playfield.pInvalidElement[I + 1] = EMERALD_RUBY;
                    Playfield.pLastStatusAnimation[I + 1] = EMERALD_ANIM_RIGHT;
                    Playfield.pStatusAnimation[I + 1] = EMERALD_ANIM_CLEAN_LEFT;
                    // Aktuelles Element auf Animation "rechts"
                    Playfield.pStatusAnimation[I] = EMERALD_ANIM_RIGHT;
                }
            } else {
                // SDL_Log("Ruby sleeps, Hitelement = %x",uHitElement);
            }
        }
    }
}



