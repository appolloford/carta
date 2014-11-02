#include "Data/Layout.h"
#include "Util.h"

#include <QDir>
#include <QDebug>

const QString Layout::LAYOUT = "Layout";
const QString Layout::LAYOUT_ROWS = "rows";
const QString Layout::LAYOUT_COLS = "cols";
const QString Layout::LAYOUT_PLUGINS = "plugins";
const QString Layout::CLASS_NAME = "Layout";
bool Layout::m_registered =
    ObjectManager::objectManager()->registerClass ( CLASS_NAME,
                                                   new Layout::Factory());

Layout::Layout( const QString& path, const QString& id):
    CartaObject( CLASS_NAME, path, id ){
    _initializeDefaultState();
    _initializeCommands();
}

void Layout::_initializeCommands(){
    addCommandCallback( "setLayoutSize", [=] (const QString & /*cmd*/,
                const QString & params, const QString & /*sessionId*/) -> QString {
        QList<QString> keys = {"rows", "cols"};
        QVector<QString> dataValues = Util::parseParamMap( params, keys );
        if ( dataValues.size() == keys.size()){
            QString rowStr = dataValues[0];
            QString colStr = dataValues[1];
            bool valid = false;
            int rows = rowStr.toInt( &valid );
            if ( valid ){
                int cols = colStr.toInt( &valid );
                if ( valid ){
                    _setLayoutSize( rows, cols);
                }
                else {
                    qDebug() << "Invalid layout cols: "<<params;
                }
            }
            else {
                qDebug() << "Invalid layout rows: "<<params;
            }
        }
        return "";
    });

    addCommandCallback( "setPlugin", [=] (const QString & /*cmd*/,
                        const QString & params, const QString & /*sessionId*/) -> QString {
        QList<QString> keys = {"names"};
        QVector<QString> dataValues = Util::parseParamMap( params, keys );
        if ( dataValues.size() == keys.size()){
            QStringList names = dataValues[0].split(".");
            bool valid = _setPlugin( names );
            if ( !valid ){
                qDebug() << "Invalid layout params: "<<params;
            }
        }
        return "";
    });
}

void Layout::_initializeDefaultState(){
    m_state.insertArray( LAYOUT_PLUGINS, 4 );
    m_state.insertValue<int>( LAYOUT_ROWS, 2 );
    m_state.insertValue<int>( LAYOUT_COLS, 2 );
    QStringList pluginNames = {"CasaImageLoader", "plugins", "Hidden", "animator"};
    _setPlugin( pluginNames );
}

void Layout::clear(){
    int oldRows = m_state.getValue<int>( LAYOUT_ROWS );
    int oldCols = m_state.getValue<int>( LAYOUT_COLS );
    if ( oldRows != 0 || oldCols != 0 ){
        m_state.setValue<int>( LAYOUT_ROWS, 0 );
        m_state.setValue<int>( LAYOUT_COLS, 0 );
        m_state.resizeArray( LAYOUT_PLUGINS, 0 );
        m_state.flushState();
    }
}

bool Layout::_setPlugin( const QStringList& names ){
    int rows = m_state.getValue<int>( LAYOUT_ROWS );
    int cols = m_state.getValue<int>( LAYOUT_COLS );
    bool valid = true;
    if ( names.size() == rows * cols ){
        int nameCount = names.size();
        for ( int i = 0; i < nameCount; i++ ){
            QString lookup( LAYOUT_PLUGINS + StateInterface::DELIMITER + QString::number(i) );
            m_state.setValue<QString>(lookup, names[i]);
        }
        m_state.flushState();
    }
    else {
        qDebug() << "Error plugin size "<< names.size() << "must match grid size="<<(rows * cols);
        valid = false;
    }
    return valid;
}

bool Layout::_setLayoutSize( int rows, int cols ){
    bool valid = false;
    if ( rows >= 0 && cols >= 0 ){
        int oldRows = m_state.getValue<int>( LAYOUT_ROWS );
        int oldCols = m_state.getValue<int>( LAYOUT_COLS );
        if ( rows != oldRows || cols != oldCols ){
            m_state.resizeArray( LAYOUT_PLUGINS, rows * cols );
            m_state.setValue<int>( LAYOUT_ROWS, rows );
            m_state.setValue<int>( LAYOUT_COLS, cols );
            m_state.flushState();
        }
        valid = true;
    }
    else {
        qDebug() << "Invalid layout rows ="<<rows<<" and/or cols="<<cols;
    }
    return valid;
}

Layout::~Layout(){

}
