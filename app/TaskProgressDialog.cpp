#include "TaskProgressDialog.h"

TaskProgressDialog::TaskProgressDialog(TaskProcessor &processor, QWidget *parent)
    :QProgressDialog( parent )
{
    setRange(0,0);
    setMinimumWidth(400);
    setMinimumDuration( 0 );
    reset();
    connect( this, &QProgressDialog::canceled, &processor, &TaskProcessor::clear );
    connect( &processor, &TaskProcessor::startingTask, this, [&]( std::shared_ptr<Task> task ){
        reset();
        show();
        connect( &task->callback(), &QtCallback::notified, this, [&]( Severity s, const QString& msg ){
            if( s == Severity::Info ){
                lastInfo_ = msg;
                setLabelText( msg );
            } else if( s == Severity::Progress ){
                setLabelText( lastInfo_ + "\n" + msg );
            }
        });
    });
    connect( &processor, &TaskProcessor::finishedTask, this, [&](){
        if( processor.getScheduledTasks().empty() ){
            hide();
        }
    });
}
