
TEMPLATE = subdirs
CONFIG -= ordered
SUBDIRS += qmultithreadnetwork samples
qmultithreadnetwork.file = source/QMultiThreadNetwork.pro
samples.depends = qmultithreadnetwork

OTHER_FILES += README.md
#EssentialDepends = 
#OptionalDepends =
