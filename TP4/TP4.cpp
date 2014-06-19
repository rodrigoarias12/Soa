#include "TP4.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>

TP4::TP4()
{
    QLabel* l = new QLabel( this );
    l->setText( "Hello World!" );
    setCentralWidget( l );
    QAction* a = new QAction(this);
    a->setText( "Quit" );
    connect(a, SIGNAL(triggered()), SLOT(close()) );
    menuBar()->addMenu( "File" )->addAction( a );
}

TP4::~TP4()
{}

#include "TP4.moc"
