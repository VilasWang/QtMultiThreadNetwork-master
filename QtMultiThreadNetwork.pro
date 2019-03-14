
TEMPLATE = subdirs
CONFIG -= ordered
SUBDIRS += qmultithreadnetwork samples
qmultithreadnetwork.file = src/QMultiThreadNetwork.pro
samples.depends = qmultithreadnetwork

OTHER_FILES += README.md
#EssentialDepends = 
#OptionalDepends =
