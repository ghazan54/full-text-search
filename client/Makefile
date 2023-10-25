APP_NAME:=fts

build:
	CGO_LDFLAGS="-L ${slib_path} -l$(libname) -Wl,-rpath,$(slib_path)" CGO_CFLAGS="-I $(include_path)" go build -o $(APP_NAME) cmd/main.go

.PHONY: run clean

run:
	./$(APP_NAME)

clean:
	$(RM) $(APP_NAME)
